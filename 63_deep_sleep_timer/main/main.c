#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_sleep.h"

RTC_DATA_ATTR int timeWokenUp = 0;

void app_main(void)
{
  esp_sleep_enable_timer_wakeup(5 * 1000000);
  printf("going to sleep, woke up %d times\n", timeWokenUp++);
  esp_deep_sleep_start();
  printf("waking up\n");
  vTaskDelay(pdMS_TO_TICKS(1000));
}
