

#include "soc/soc.h"
#include "soc/gpio_sig_map.h"
#include "soc/i2s_reg.h"
#include "soc/i2s_struct.h"
#include "soc/io_mux_reg.h"
#include "driver/gpio.h"
#include "driver/periph_ctrl.h"
#include "rom/lldesc.h"
#include "i2s_wrapper.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "rom/gpio.h"

#define VGA_X 640
#define VGA_Y 480

#define downscale_x 2
#define downscale_y 2

#define FRAME_X VGA_X/downscale_x //640
#define FRAME_Y VGA_Y/downscale_y //480

#define SM_0A0B_0C0D 1

#define VSYNC_PIN 39
#define PLCK_PIN 23
#define HSYNC_PIN 13

#define CAMERA_PINS_NUM 11

#define D0 18
#define D1 19
#define D2 25
#define D3 26
#define D4 27
#define D5 34
#define D6 35
#define D7 36

//uint8_t camera_data_pins [CAMERA_PINS_NUM] = {D0, D1, D2, D3, D4, D5, D6, D7, VSYNC_PIN, HSYNC_PIN, HSYNC_PIN};



lldesc_t ring_buffer_descriptors[2];
uint8_t ring_buffers [2][VGA_X*2];
uint8_t active_ring_buffer;


intr_handle_t i2sInterruptHandle = 0;

uint8_t * buffer_frame;
uint32_t frame_pointer;


bool frame_received;


static inline void i2sConfReset()
  {
    const uint32_t lc_conf_reset_flags = I2S_IN_RST_M | I2S_AHBM_RST_M | I2S_AHBM_FIFO_RST_M;
    I2S0.lc_conf.val |= lc_conf_reset_flags;
    I2S0.lc_conf.val &= ~lc_conf_reset_flags;
    
    const uint32_t conf_reset_flags = I2S_RX_RESET_M | I2S_RX_FIFO_RESET_M | I2S_TX_RESET_M | I2S_TX_FIFO_RESET_M;
    I2S0.conf.val |= conf_reset_flags;
    I2S0.conf.val &= ~conf_reset_flags;
    while (I2S0.state.rx_fifo_reset_back);
  }

// static void IRAM_ATTR gpio_interrupt_handler(void *args)
// {
//     frame_received = true;
// }

uint8_t* i2sRun()
{

    frame_received = false;
    
    esp_intr_disable(i2sInterruptHandle);
    i2sConfReset();
    active_ring_buffer = 0;
    frame_pointer = 0;
    
    I2S0.rx_eof_num = ring_buffer_descriptors[0].length / 4;
    I2S0.in_link.addr = (uint32_t)&(ring_buffer_descriptors[0]);
    I2S0.in_link.start = 1;
    I2S0.int_clr.val = I2S0.int_raw.val;
    I2S0.int_ena.val = 0;
    I2S0.int_ena.in_done = 1;
    esp_intr_enable(i2sInterruptHandle);
    I2S0.conf.rx_start = 1;

    while(!frame_received); 

    return buffer_frame;
}



void i2sStop()
{
    esp_intr_disable(i2sInterruptHandle);
    i2sConfReset();
    I2S0.conf.rx_start = 0;
    frame_received = true;
}



static void IRAM_ATTR i2sInterrupt(void* arg)
{
    //frame_received = true;
    I2S0.int_clr.val = I2S0.int_raw.val;

    active_ring_buffer = (active_ring_buffer+1) % downscale_y;

    if (active_ring_buffer)
        return;
    
    for(int i = 0; i < VGA_X * 2; i += 2*downscale_x)
        buffer_frame[frame_pointer++] = ring_buffers[0][i];

    if (frame_pointer == FRAME_Y*FRAME_X)
        i2sStop();
}






void i2sInit();
void dmaBufferInit();


void init_i2s()
{


    buffer_frame = (uint8_t *) malloc(FRAME_X * FRAME_Y * sizeof(uint8_t));

    if (!buffer_frame)
        printf("\n\nsad\n\n");

    i2sInit();
    dmaBufferInit();  //two bytes per dword packing, two bytes per pixel
}


void i2sInit(){    
    // Route input GPIOs to I2S peripheral using GPIO matrix, last parameter is invert
    gpio_matrix_in(D0,    I2S0I_DATA_IN0_IDX, false);
    gpio_matrix_in(D1,    I2S0I_DATA_IN1_IDX, false);
    gpio_matrix_in(D2,    I2S0I_DATA_IN2_IDX, false);
    gpio_matrix_in(D3,    I2S0I_DATA_IN3_IDX, false);
    gpio_matrix_in(D4,    I2S0I_DATA_IN4_IDX, false);
    gpio_matrix_in(D5,    I2S0I_DATA_IN5_IDX, false);
    gpio_matrix_in(D6,    I2S0I_DATA_IN6_IDX, false);
    gpio_matrix_in(D7,    I2S0I_DATA_IN7_IDX, false);
    gpio_matrix_in(0x30,  I2S0I_DATA_IN8_IDX, false);
    gpio_matrix_in(0x30,  I2S0I_DATA_IN9_IDX, false);
    gpio_matrix_in(0x30,  I2S0I_DATA_IN10_IDX, false);
    gpio_matrix_in(0x30,  I2S0I_DATA_IN11_IDX, false);
    gpio_matrix_in(0x30,  I2S0I_DATA_IN12_IDX, false);
    gpio_matrix_in(0x30,  I2S0I_DATA_IN13_IDX, false);
    gpio_matrix_in(0x30,  I2S0I_DATA_IN14_IDX, false);
    gpio_matrix_in(0x30,  I2S0I_DATA_IN15_IDX, false);
        
    gpio_matrix_in(0x38, I2S0I_V_SYNC_IDX, false);
    gpio_matrix_in(0x38,  I2S0I_H_SYNC_IDX, false);  //0x30 sends 0, 0x38 sends 1
    gpio_matrix_in(0x38,  I2S0I_H_ENABLE_IDX, false);
    gpio_matrix_in(PLCK_PIN,  I2S0I_WS_IN_IDX, false);

    // Enable and configure I2S peripheral
    periph_module_enable(PERIPH_I2S0_MODULE);
    
    // Toggle some reset bits in LC_CONF register
    // Toggle some reset bits in CONF register
    i2sConfReset();
    // Enable slave mode (sampling clock is external)
    I2S0.conf.rx_slave_mod = 1;
    // Enable parallel mode
    I2S0.conf2.lcd_en = 1;
    // Use HSYNC/VSYNC/HREF to control sampling
    I2S0.conf2.camera_en = 1;
    // Configure clock divider
    I2S0.clkm_conf.clkm_div_a = 1;
    I2S0.clkm_conf.clkm_div_b = 0;
    I2S0.clkm_conf.clkm_div_num = 2;
    // FIFO will sink data to DMA
    I2S0.fifo_conf.dscr_en = 1;
    // FIFO configuration
    //two bytes per dword packing
    I2S0.fifo_conf.rx_fifo_mod = SM_0A0B_0C0D;  //pack two bytes in one dword see :https://github.com/igrr/esp32-cam-demo/issues/29
    I2S0.fifo_conf.rx_fifo_mod_force_en = 1;
    I2S0.conf_chan.rx_chan_mod = 1;
    // Clear flags which are used in I2S serial mode
    I2S0.sample_rate_conf.rx_bits_mod = 0;
    I2S0.conf.rx_right_first = 0;
    I2S0.conf.rx_msb_right = 0;
    I2S0.conf.rx_msb_shift = 0;
    I2S0.conf.rx_mono = 0;
    I2S0.conf.rx_short_sync = 0;
    I2S0.timing.val = 0;

    // Allocate I2S interrupt, keep it disabled
    esp_intr_alloc(ETS_I2S0_INTR_SOURCE, ESP_INTR_FLAG_INTRDISABLED | ESP_INTR_FLAG_LEVEL1 | ESP_INTR_FLAG_IRAM, &i2sInterrupt, NULL, &i2sInterruptHandle);
    
}


void dmaBufferInit()
{

    ring_buffer_descriptors[0].length = VGA_X*2;
    ring_buffer_descriptors[0].size = ring_buffer_descriptors[0].length;
    ring_buffer_descriptors[0].owner = 1;
    ring_buffer_descriptors[0].sosf = 1;
    ring_buffer_descriptors[0].buf = (uint8_t*) ring_buffers[0];
    ring_buffer_descriptors[0].offset = 0;
    ring_buffer_descriptors[0].empty = 0;
    ring_buffer_descriptors[0].eof = 1;
    ring_buffer_descriptors[0].qe.stqe_next = &ring_buffer_descriptors[1];

    ring_buffer_descriptors[1].length = VGA_X*2;
    ring_buffer_descriptors[1].size = ring_buffer_descriptors[1].length;
    ring_buffer_descriptors[1].owner = 1;
    ring_buffer_descriptors[1].sosf = 1;
    ring_buffer_descriptors[1].buf = (uint8_t*) ring_buffers[1];
    ring_buffer_descriptors[1].offset = 0;
    ring_buffer_descriptors[1].empty = 0;
    ring_buffer_descriptors[1].eof = 1;
    ring_buffer_descriptors[1].qe.stqe_next = &ring_buffer_descriptors[0];

}