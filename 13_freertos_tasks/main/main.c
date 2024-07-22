#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void task1(void *params)
{
  while (true)
  {
    printf("%s: reading temperature\n", (char *)params);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void task2(void *params)
{
  while (true)
  {
    printf("%s: reading humidity\n", (char *)params);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

void app_main(void)
{
  // xTaskCreate(&task1, "temperature reading", 2048, NULL, 2, NULL);
  // xTaskCreate(&task2, "humidity reading", 2048, NULL, 2, NULL);

  xTaskCreate(&task1, "temperature reading", 2048, "Task 1", 2, NULL);
  xTaskCreate(&task2, "humidity reading", 2048, "Task 2", 2, NULL);

}
