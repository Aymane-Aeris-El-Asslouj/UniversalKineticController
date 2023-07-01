#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "driver/gptimer.h"

#include "timer_PPS.h"
#include "GNSS_PPS.h"

#include <stdlib.h>

#define PRECISION 1000


int64_t time_counter = 0;

gptimer_handle_t pps_timer_handle = NULL;

int64_t extended_alarm_offset = 0;
int64_t old_alarm_count = TIMER_RESOLUTION;

int64_t captured_time_points = 0;
int64_t old_err = 0;
int64_t i_err = 0;


int64_t random_percent(int64_t chance)
{

    int64_t sign = (chance >= 0) ? 1 : -1;

    if (chance < 0)
        chance = -chance;

    if (chance > (rand() % PRECISION))
        return sign;
    else
        return 0;
}

int64_t alarm_count_from_period(int64_t period)
{
    old_alarm_count = TIMER_RESOLUTION + period/PRECISION + random_percent(period - PRECISION*(period/PRECISION));
    return old_alarm_count;
}

static bool IRAM_ATTR my_alarm (gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx)
{


    // Count the number of time points received
    if (captured_time_points < MINIMUM_SAMPLES_FOR_TUNING)
        captured_time_points++;

    // Move the offset to the int64_terval [-alarm_period/2, alarm_period/2]
    int64_t phase_offset = gnss_pps_get_time_point();
    if (phase_offset < old_alarm_count/2)
        phase_offset = phase_offset;
    else
        phase_offset = phase_offset - old_alarm_count;
    phase_offset = phase_offset % old_alarm_count;

    // Get offset of 
    int64_t err = -phase_offset;


    if (time_counter < 450 || time_counter > 450+3600*11)
        i_err += err;
    int64_t d_err = err - old_err;
    old_err = err;

    int64_t up = b*c*d_err+a*c*err +a*b*i_err;
    int64_t down = a*b*c;

    

    // Adjust frequency if there are at least 2 gnss pps time points
    if (captured_time_points == MINIMUM_SAMPLES_FOR_TUNING){

        if (time_counter < 450 || time_counter > 450+3600*11)
           extended_alarm_offset += (PRECISION * up)/down;

        gptimer_alarm_config_t alarm_config = {
            .alarm_count = alarm_count_from_period(extended_alarm_offset), 
            .reload_count = 0, 
            .flags.auto_reload_on_alarm = 1
        };
        gptimer_set_alarm_action(timer, &alarm_config);
    }

    return true;
}

// Initialize 1PPS timer
void timer_PPS_init()
{
    // Set timer to have maximum resolution counting down
    gptimer_config_t config =  {
        .resolution_hz = TIMER_RESOLUTION,
        .direction = GPTIMER_COUNT_UP,  /*!< Counter direction  */
        .clk_src = TIMER_CLK_SRC
    };
    ESP_ERROR_CHECK(gptimer_new_timer(&config, &pps_timer_handle));


    // Set nominal 1Hz reloaded alarm
    gptimer_alarm_config_t alarm_config = {
        .alarm_count = alarm_count_from_period(extended_alarm_offset), 
        .reload_count = 0, 
        .flags.auto_reload_on_alarm = 1
    };
    gptimer_set_alarm_action(pps_timer_handle, &alarm_config);

    // Add alarm handler to timer
    gptimer_event_callbacks_t cbs = {
        .on_alarm = my_alarm, // register user callback
    };
    gptimer_register_event_callbacks(pps_timer_handle, &cbs, NULL);

    // enable timer
    gptimer_enable(pps_timer_handle);
}

// Start timer
void timer_pps_start_timer()
{
    // Start timer
    gptimer_start(pps_timer_handle);
}

// Get current pps timer counter value
int64_t timer_pps_get_time()
{
    int time_point;
    gptimer_get_raw_count(pps_timer_handle, &time_point);
    return time_point;
}

// Return alarm period
int64_t timer_pps_get_alarm_period()
{
    return old_alarm_count;
}

// print alarm period
void print_alarm_period()
{
    //if (captured_time_points > 0)
        printf("%lld - extended_alarm_offset: &%lld&,      int64_t_err: %lld     old_err: $%lld$     ", time_counter++, extended_alarm_offset, i_err, old_err);
}