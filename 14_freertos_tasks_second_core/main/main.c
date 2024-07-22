#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


void task1(void *params)
{
  while(true)
  {
    printf("%s: Reading temperature\n", (char*)params);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void task2(void *params)
{
  while(true)
  {
    printf("%s: Reading Humidity\n", (char*)params);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}


void app_main(void)
{
  xTaskCreate(&task1, "temperature reading", 2048, "Task 1", 2, NULL);
  xTaskCreatePinnedToCore(&task2, "humidity reading", 2048, "Task 2", 2, NULL, 1); // 0 here will same like as before but 1 will give the ability to use second core
}
