#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include "esp_log.h"
#include "xclk.h"



    
void camera_disable_out_clock()
{
    periph_module_disable(PERIPH_LEDC_MODULE);
}
