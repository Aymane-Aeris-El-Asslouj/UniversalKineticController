
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "driver/gpio.h"
#include "driver/gptimer.h"

#include "timer_PPS.h"
#include "GNSS_PPS.h"


int gnss_pps_time_point = 0;

bool first_pulse = true;
// Records the timer count values (time points) at which the pps signal was received 
void IRAM_ATTR pps_posedge_handler(void *arg)
{
    // start timer if this is the first pulse
    if (first_pulse){
        first_pulse = false;
        timer_pps_start_timer();
        return;
    }

    // Get phase offset
    gnss_pps_time_point = timer_pps_get_time();
}


// Initialize gnss pps input handler
void GNSS_PPS_init()
{

    // Set PPS pin as pull-up input with posedge interrupt
    gpio_reset_pin(PPS_PIN);
    gpio_set_direction(PPS_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(PPS_PIN, GPIO_PULLUP_ONLY);
    gpio_set_intr_type(PPS_PIN, GPIO_INTR_POSEDGE);

    // Add the pps posedge handler to the pps pin
    ESP_ERROR_CHECK(gpio_install_isr_service(ESP_INTR_FLAG_LEVEL1));
    ESP_ERROR_CHECK(gpio_isr_handler_add(PPS_PIN, pps_posedge_handler, NULL));
}

// print gnss time offset information
void print_gnss_offset()
{
    int phase_offset = gnss_pps_time_point;
    int given_alarm_period = timer_pps_get_alarm_period();
    if (phase_offset < given_alarm_period/2)
        phase_offset = phase_offset;
    else
        phase_offset = phase_offset - given_alarm_period;
    printf("time point: %d", phase_offset % given_alarm_period);
}

// Return gnss time point
int gnss_pps_get_time_point()
{
    return gnss_pps_time_point;
}