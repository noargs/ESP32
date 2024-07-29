#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define       PIN_SWITCH        26
#define       PIN_LED           25

void app_main(void)
{
  // gpio_pad_select_gpio(PIN_LED);      // Pre-IDF v5
  // gpio_pad_select_gpio(PIN_SWITCH);   // Pre-IDF v5

  gpio_set_direction(PIN_LED, GPIO_MODE_OUTPUT);

  gpio_set_direction(PIN_SWITCH, GPIO_MODE_INPUT);
  gpio_pulldown_en(PIN_SWITCH);
  gpio_pullup_dis(PIN_SWITCH);

  while (true)
  {
    int level = gpio_get_level(PIN_SWITCH);
    gpio_set_level(PIN_LED, level);
    vTaskDelay(1);
  }
}
