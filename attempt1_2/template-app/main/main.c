#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"

#define BUFFER_SIZE 1024
#define INPUT_1    GPIO_NUM_9
#define INPUT_2    GPIO_NUM_15

static const char* TAG = "ATTEMPT1_2";

void app_main(void)
{
    ESP_ERROR_CHECK(gpio_reset_pin(INPUT_1));
    ESP_ERROR_CHECK(gpio_set_direction(INPUT_1, GPIO_MODE_INPUT));

    ESP_ERROR_CHECK(gpio_reset_pin(INPUT_2));
    ESP_ERROR_CHECK(gpio_set_direction(INPUT_2, GPIO_MODE_INPUT));

    const uart_port_t uart_num = UART_NUM_1;
    uart_config_t uart_config = {
        .baud_rate  = 115200,
        .data_bits  = UART_DATA_8_BITS,
        .parity     = UART_PARITY_DISABLE,
        .stop_bits  = UART_STOP_BITS_1,
        .flow_ctrl  =  UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT
    };

    int buttonDown = 0;

    char tx_chr;

    ESP_ERROR_CHECK(uart_driver_install(uart_num, BUFFER_SIZE, 0, 0, NULL, 0));
    
    ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));

    ESP_ERROR_CHECK(uart_set_pin(uart_num, 4, 5, 18, 19));

    while(1) {
        if(gpio_get_level(INPUT_1) && !buttonDown) {
            ESP_LOGI(TAG, "Button Pressed!");
            buttonDown = 1;
            tx_chr = 'a';
            uart_write_bytes(uart_num, &tx_chr, 1);
        } else if(gpio_get_level(INPUT_2) && !buttonDown) {
            ESP_LOGI(TAG, "Off Button Pressed!");
            buttonDown = 1;
            tx_chr = 'b';
            uart_write_bytes(uart_num, &tx_chr, 1);
        } else if(!gpio_get_level(INPUT_1) && !gpio_get_level(INPUT_2) && buttonDown) {
            buttonDown = 0;
        }
        vTaskDelay(1);
    }
}