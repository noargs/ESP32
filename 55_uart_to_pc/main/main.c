#include <stdio.h>
#include <string.h>

#include "esp_log.h"
#include "driver/uart.h"

#define TXD_PIN    4
#define RXD_PIN    5
#define UART_NUM   UART_NUM_1

#define RX_BUF_SIZE   1024

QueueHandle_t uart_queue;

void app_main(void)
{
  uart_config_t uart_config = {
    .baud_rate = 9600,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
  };
  uart_param_config(UART_NUM, &uart_config);
  uart_set_pin(UART_NUM, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
  uart_driver_install(UART_NUM, RX_BUF_SIZE, 0, 0, NULL, 0);

  char message[] = "ping";
  printf("sending: %s\n", message);
  uart_write_bytes(UART_NUM, message, sizeof(message));

  char inco_message[RX_BUF_SIZE];
  while (true)
  {
    memset(inco_message, 0, sizeof(inco_message));
    uart_read_bytes(UART_NUM, (uint8_t*)inco_message, RX_BUF_SIZE, pdMS_TO_TICKS(500));
    printf("received: %s\n", inco_message);
    uart_write_bytes(UART_NUM, inco_message, sizeof(inco_message));
  }

}
