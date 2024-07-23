#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

QueueHandle_t queue;

void listenForHTTP(void *params)
{
  int count = 0;
  while (true)
  {
    count++;
    printf("received http message\n");
    long ok = xQueueSend(queue, &count, 1000/portTICK_PERIOD_MS);
    if (ok)
    {
      printf("added message to queue\n");
    }
    else
    {
      printf("failed to add message to queue\n");
    }
    vTaskDelay(1000/portTICK_PERIOD_MS);
  }
}

void task1(void *params)
{
  int rxInt;
  while (true)
  {
    if (xQueueReceive(queue, &rxInt, 4000/portTICK_PERIOD_MS))
    {
      printf("doing something with http %d\n", rxInt);
    }
  }
}

void app_main(void)
{
  queue = xQueueCreate(3, sizeof(int));
  xTaskCreate(&listenForHTTP, "get http", 2048, NULL, 1, NULL);
  xTaskCreate(&task1, "do something with http", 2048, NULL, 1, NULL);
}
