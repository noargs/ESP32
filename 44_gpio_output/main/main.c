#include <stdio.h>

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define PIN        26

void app_main(void)
{
  // gpio_pad_select_gpio(PIN);    pre-IDF v5.1
  gpio_set_direction(PIN, GPIO_MODE_OUTPUT);
  int isON=0;

  while (true)
  {
    isON = !isON;
    gpio_set_level(PIN, isON);
    vTaskDelay(1000/portTICK_PERIOD_MS);
  }
}
