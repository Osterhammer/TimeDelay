/* THREE_POSITION_2TOF/main.cpp - Application main entry point */

/*
 * Copyright (c) 2017 Intel Corporation
 * Additional Copyright (c) 2018 Espressif Systems (Shanghai) PTE LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "Automation_THREE_POSITION.h"
#include "AutomationTimer_THREE_POSITION_2TOF.h"

#define LOG_LOCAL_LEVEL ESP_LOG_INFO
#include "esp_log.h"


static const char * const TAG = "THREE_POSITION_2TOF";
static int I1 = 0;
static int I2 = 0;

#define BUTTON_I1 GPIO_NUM_26        // Pin 26.
#define BUTTON_I2 GPIO_NUM_32        // Pin 32.
#define GPIO_Q1 GPIO_NUM_19
#define GPIO_Q2 GPIO_NUM_23






/* Inside .cpp file, app_main function must be declared with C linkage */
extern "C" void app_main(void)
{



    ESP_LOGI(TAG, "Initializing...");

    /* Configure the IOMUX register for pad BLINK_GPIO (some pads are
       muxed to GPIO on reset already, but some default to other
       functions and need to be switched to GPIO. Consult the
       Technical Reference for a list of pads and their default
       functions.)
    */
    gpio_reset_pin(GPIO_Q1);
    gpio_reset_pin(GPIO_Q2);
    gpio_reset_pin(BUTTON_I1);
    gpio_reset_pin(BUTTON_I2);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(GPIO_Q1, GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO_Q2, GPIO_MODE_OUTPUT);
    gpio_set_direction(BUTTON_I1, GPIO_MODE_INPUT);
    gpio_set_direction(BUTTON_I2, GPIO_MODE_INPUT);
    gpio_set_level(GPIO_Q1, 0); //set to 0 at Reset.
    gpio_set_level(GPIO_Q2, 0); //set to 0 at Reset.







    THREE_POSITION_SWITCH SWITCH;
    THREE_POSITION_2TOF    TIMER;
    THREE_POSITION_VALVE     VALVE;
    TIMER.PT_up = 1000;
    TIMER.PT_down = 500;


    while (true) {
        I1 = not gpio_get_level(BUTTON_I1);
        I2 = not gpio_get_level(BUTTON_I2);



        SWITCH.I1 = I1;
        SWITCH.I2 = I2;
        SWITCH();
        TIMER.IN = SWITCH.State;
        TIMER();
        VALVE.State = TIMER.OUT;
        VALVE();




        gpio_set_level(GPIO_Q1, VALVE.Q1);
        gpio_set_level(GPIO_Q2, VALVE.Q2);


        vTaskDelay(100 / portTICK_PERIOD_MS); // 100ms cycle for Test.

    }
}

