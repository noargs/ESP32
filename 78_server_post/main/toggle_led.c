#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "driver/gpio.h"
#include "toggle_led.h"

#define LED 2

void init_led(void)
{
  gpio_set_direction(LED, GPIO_MODE_OUTPUT);
}

void toggle_led(bool is_on)
{
  gpio_set_level(LED, is_on);
}