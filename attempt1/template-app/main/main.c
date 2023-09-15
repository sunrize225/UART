#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"

#define BUFFER_SIZE 1024
#define BLUE_LED    GPIO_NUM_17

static const char* TAG = "ATTEMPT1";

void app_main(void)
{
    ESP_ERROR_CHECK(gpio_reset_pin(BLUE_LED));
    ESP_ERROR_CHECK(gpio_set_direction(BLUE_LED, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_level(BLUE_LED, 0));


    const uart_port_t uart_num = UART_NUM_1;
    uart_config_t uart_config = {
        .baud_rate  = 115200,
        .data_bits  = UART_DATA_8_BITS,
        .parity     = UART_PARITY_DISABLE,
        .stop_bits  = UART_STOP_BITS_1,
        .flow_ctrl  =  UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT
    };
    uint8_t *data = malloc(BUFFER_SIZE); 
    int len;

    ESP_ERROR_CHECK(uart_driver_install(uart_num, BUFFER_SIZE, 0, 0, NULL, 0));
    
    ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));

    ESP_ERROR_CHECK(uart_set_pin(uart_num, 4, 5, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    while(1) {

        len = uart_read_bytes(uart_num, data, BUFFER_SIZE - 1, 100);

        if(len) {
            ESP_LOGI(TAG, "DATA RECEIVED!");
            if(data[0] == 'a') {
                ESP_LOGI(TAG, "LED ON");
                ESP_ERROR_CHECK(gpio_set_level(BLUE_LED, 1));
            } else if(data[0] == 'b') {
                ESP_LOGI(TAG, "LED OFF");
                ESP_ERROR_CHECK(gpio_set_level(BLUE_LED, 0));
            } else {
                ESP_LOGI(TAG, "%c", data[0]);
            }
        }

        vTaskDelay(1);
    }
}