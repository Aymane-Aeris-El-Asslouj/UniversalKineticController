#include "OV7670_driver.h"
#include "driver/ledc.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "i2c_wrapper.h"
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include "soc/i2s_reg.h"
#include "i2s_wrapper.h"

#define CAMERA_CLOCK_FREQUENCY_FROM_PWM 24000000

#define OV7670_ADDRESS 0x21

#define FRAME_X 640 //640
#define FRAME_Y 240 //480

#define VSYNC_PIN 39
#define PLCK_PIN 23
#define HSYNC_PIN 13

#define CAMERA_PINS_NUM 11

uint8_t camera_data_pins [CAMERA_PINS_NUM] = {18, 19, 25, 26, 27, 34, 35, 36, VSYNC_PIN, HSYNC_PIN, PLCK_PIN};

uint8_t * frame;

ledc_timer_config_t ledc_timer = {
    .speed_mode = LEDC_HIGH_SPEED_MODE,
    .timer_num  = LEDC_TIMER_0,
    .duty_resolution = LEDC_TIMER_1_BIT,
    .freq_hz = CAMERA_CLOCK_FREQUENCY_FROM_PWM,
    .clk_cfg = LEDC_AUTO_CLK
};

ledc_channel_config_t ledc_channel = {
    .channel = LEDC_CHANNEL_0,
    .gpio_num  = 22,
    .speed_mode = LEDC_HIGH_SPEED_MODE,
    .timer_sel = LEDC_TIMER_0,
    .intr_type = LEDC_INTR_DISABLE,
    .duty = 1,
    .hpoint = 0
};

void camera_write_config(const struct regval_list reglist[])
{
    const struct regval_list *next = reglist;
	while(1){
		uint8_t reg_addr = next->reg_num;
		uint8_t reg_val = next->value;
		if((reg_addr==255)&&(reg_val==255))
			break;
		i2c_write_reg(reg_addr, reg_val);
		next++;
	}
}

void camera_vga_config()
{
    // Reset parameters
    i2c_write_reg(0x12, 0x80);
    vTaskDelay(100 / portTICK_PERIOD_MS); 

    // Write default paramters
	camera_write_config(ov7670_default_regs);

    // Something? //PCLK does not toggle on HBLANK.
	i2c_write_reg(REG_COM10, 32);

    // Set resolution to VGA
    i2c_write_reg(REG_COM3,0);	// REG_COM3
	camera_write_config(vga_ov7670);

    // Set color space to Bayer RGB
    camera_write_config(bayerRGB_ov7670);

    // Something?
	i2c_write_reg(0x11,0x40);

	i2c_write_reg(REG_COM8, COM8_FASTAEC|COM8_AECSTEP|COM8_AEC|COM8_AWB);
	i2c_write_reg(REG_GAIN, 0xff);
	i2c_write_reg(REG_VREF,0xca);	// VREF
	i2c_write_reg(REG_AECHH,0x3f);
	i2c_write_reg(REG_BRIGHT,0xff);

    
    //vTaskDelay(2000 / portTICK_PERIOD_MS); 
}

void camera_init(uint8_t sda_pin, uint8_t scl_pin, uint8_t xclk_pin)
{

    // Setup Camera system clock with PWM
    ledc_channel.gpio_num = xclk_pin;
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));

    // Initialized I2C bus
    i2c_master_init(OV7670_ADDRESS, sda_pin, scl_pin);

    // Write VGA config to camera
    camera_vga_config();

    // Configure camera input pins (data, vsync, plck)
    for(int i = 0; i < CAMERA_PINS_NUM; i++)
    {
        gpio_set_direction(camera_data_pins[i], GPIO_MODE_INPUT);
        gpio_set_pull_mode(camera_data_pins[i], GPIO_FLOATING);
        gpio_set_intr_type(camera_data_pins[i], GPIO_INTR_DISABLE);
    }

    printf("hi222");
    init_i2s();
    printf("hi2222");
}

inline void get_sync()
{
    while (1)
    {
        vTaskDelay(1 / portTICK_PERIOD_MS);
        // scanf("%c", &char_me);
        //uart_read_bytes(0, &char_me, 1, 100 / portTICK_PERIOD_MS);
        if (getchar() == 'f')
            return;
    }
}

void print_img()
{
    // uint8_t line [FRAME_X+1];
    // line[FRAME_X] ='\0';

    printf("new image:\n");
    for(int j = 0; j < FRAME_Y; j++)
    {
        for(int i = 0; i < FRAME_X; i++)
        {
            // if (frame[FRAME_X * j + i] == 0)
            //  frame[FRAME_X * j + i] = 1;
            printf("%d ", frame[FRAME_X * j + i]);
        }
        printf("\n");
        if (j % 10 == 0)
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }


    // for(int j = 0; j < FRAME_Y; j++)
    // {
    //     //get_sync();
    //     strncpy((char *)line, (char *)(&frame[FRAME_X * j]), FRAME_X);
    //     printf("%s", line);
    //     fflush(stdout);
    // }
}

uint8_t* captureImg(uint16_t wg,uint16_t hg)
{
    

    // Wait for Vsync signal for new image
    while(!(gpio_get_level(VSYNC_PIN)));//wait for high
    while((gpio_get_level(VSYNC_PIN)));//wait for low

    frame = i2sRun();

    return frame;

    
}

