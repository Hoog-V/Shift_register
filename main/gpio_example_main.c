/* GPIO Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

/**
 * Brief:
 * This test code shows how to configure gpio and how to use gpio interrupt.
 *
 * GPIO status:
 * GPIO18: output
 * GPIO19: output
 * GPIO4:  input, pulled up, interrupt from rising edge and falling edge
 * GPIO5:  input, pulled up, interrupt from rising edge.
 *
 * Test:
 * Connect GPIO18 with GPIO4
 * Connect GPIO19 with GPIO5
 * Generate pulses on GPIO18/19, that triggers interrupt on GPIO4/5
 *
 */

#define GPIO_OUTPUT_OE_0    25
#define GPIO_OUTPUT_RCLK_1    26
#define GPIO_OUTPUT_SRCLK_2    27
#define GPIO_OUTPUT_SER_3    32
#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<GPIO_OUTPUT_OE_0) | (1ULL<<GPIO_OUTPUT_RCLK_1) | \
                              (1ULL<<GPIO_OUTPUT_SRCLK_2) | (1ULL<<GPIO_OUTPUT_SER_3))


void shift_bit(uint8_t bit, uint8_t val){
    static uint8_t origval = 0;
    if(val == 1)
        origval |= (1 << bit);
    else
        origval &= ~(1 << bit);
    for(uint8_t i=0; i< 7; i++){
        uint8_t lvl = (origval & (1<<i));
        gpio_set_level(GPIO_OUTPUT_SER_3, lvl);
        gpio_set_level(GPIO_OUTPUT_SRCLK_2, 1);
        gpio_set_level(GPIO_OUTPUT_SRCLK_2, 0);
    }
    gpio_set_level(GPIO_OUTPUT_RCLK_1, 1);
    gpio_set_level(GPIO_OUTPUT_RCLK_1, 0);
}
void app_main(void)
{
    //zero-initialize the config structure.
    gpio_config_t io_conf = {};
    //disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    //disable pull-down mode
    io_conf.pull_down_en = 1;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);
    //change gpio interrupt type for one pin
    int cnt = 0;
    printf("Minimum free heap size: %d bytes\n", esp_get_minimum_free_heap_size());
    gpio_set_level(GPIO_OUTPUT_RCLK_1, 0);
    gpio_set_level(GPIO_OUTPUT_OE_0, 0);
    gpio_set_level(GPIO_OUTPUT_SER_3, 0);
    gpio_set_level(GPIO_OUTPUT_SRCLK_2, 0);
    while(1){
    for(uint8_t i = 0; i< 7; i++){
    shift_bit(i, 1);
    vTaskDelay(100/portTICK_PERIOD_MS);
    }
     for(int i= 7; i > -1; i--){
     shift_bit(i, 0);
     vTaskDelay(100/portTICK_PERIOD_MS);
    }
    vTaskDelay(100/portTICK_PERIOD_MS);
    }
    }
