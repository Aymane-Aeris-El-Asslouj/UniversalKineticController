/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_flash.h"
#include "OV7670_driver.h"
#include "i2c_wrapper.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "gnss_receiver_read.h"
#include "GNSS_PPS.h"

// // Wifi server
#include <nvs_flash.h>
#include "connect_wifi.h"
#include "esp_http_server.h"


#define CAMERA_SCL_PIN 33               /*!< gpio number for I2C master clock */
#define CAMERA_SDA_PIN 32               /*!< gpio number for I2C master data  */
#define CAMERA_XCLK_PIN 22

uint8_t* custom_buf;

static const char* TAG = "example";

esp_err_t jpg_stream_httpd_handler(httpd_req_t *req){


    //printf("a\n");
    //fflush(stdout);
    uint8_t * fb = captureImg(640,480); //captureImg(640,480);
    //printf("b\n");
    //fflush(stdout);

    //httpd_resp_send(req, (const char*)words, -1);
    //printf("c\n");
    httpd_resp_send(req, (const char*)fb, 320*240);
    //printf("d\n");
    return ESP_OK;

}

esp_err_t cursor_httpd_handler(httpd_req_t *req){


     //printf("a\n");
    //fflush(stdout);
    uint8_t * fb = captureImg(640,480); //captureImg(640,480);
    //printf("b\n");
    //fflush(stdout);



    uint64_t x_sum = 0;
    uint64_t y_sum = 0;
    uint64_t n = 0;

    uint16_t x_ave = 0;
    uint16_t y_ave = 0;


    char data_upload[5] = {0, 0, 0, 0, 0};

    for (int x = 0; x < 640/2; x++)
        for (int y = 0; y < 480/2; y++)
            if (fb[y*(640/2)+x] > 100)
            {
                x_sum += x;
                y_sum += y;
                n += 1;
            }

    if (n > 1)
    {
        data_upload[0] = 1;
        x_ave = x_sum/n;
        y_ave = y_sum/n;
        data_upload[1] = (char)(x_ave / 256);
        data_upload[2] = (char)(x_ave % 256);
        data_upload[3] = (char)(y_ave / 256);
        data_upload[4] = (char)(y_ave % 256);

    }



    //httpd_resp_send(req, (const char*)words, -1);
    //printf("c\n");
    httpd_resp_send(req, data_upload, 5);
    //printf("d\n");
    return ESP_OK;

}

httpd_uri_t uri_get = {
    .uri = "/try/",
    .method = HTTP_GET,
    .handler = jpg_stream_httpd_handler,
    .user_ctx = NULL};
//jpg_stream_httpd_handler
//cursor_httpd_handler
httpd_uri_t uri_get_cursor = {
.uri = "/cursor/",
.method = HTTP_GET,
.handler = jpg_stream_httpd_handler,
.user_ctx = NULL};


httpd_handle_t setup_server(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t stream_httpd  = NULL;
    httpd_handle_t stream_httpd_2  = NULL;

    if (httpd_start(&stream_httpd , &config) == ESP_OK)
    {
        httpd_register_uri_handler(stream_httpd_2 , &uri_get_cursor);
        httpd_register_uri_handler(stream_httpd , &uri_get);
    }

    return stream_httpd;
}


extern int I;

void app_main(void)
{
    esp_err_t err;

    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    connect_wifi();

    if (wifi_connect_status)
    {
        
        camera_init(CAMERA_SDA_PIN, CAMERA_SCL_PIN, CAMERA_XCLK_PIN);
        //custom_buf = captureImg(640,480);
        
        setup_server();
        ESP_LOGI(TAG, "ESP32 CAM Web Server is up and running\n");
    }
    else
        ESP_LOGI(TAG, "Failed to connected with Wi-Fi, check your network Credentials\n");



    while(1)vTaskDelay(1000 / portTICK_PERIOD_MS); 


    camera_init(CAMERA_SDA_PIN, CAMERA_SCL_PIN, CAMERA_XCLK_PIN);


    while(1)
    {
        printf("\nNew img\n");

        ESP_LOGI(TAG, "a\n");
        captureImg(640,480);
        ESP_LOGI(TAG, "b\n");


        print_img();

        printf("End img, here is a regiter value:");
        i2c_read_reg(0x3d);
        vTaskDelay(1000 / portTICK_PERIOD_MS); 

    }
}
