#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

EventGroupHandle_t evGrp;
const int gotHttp = BIT0; // (1<<0)
const int gotBLE = BIT1;

void listenForHTTP(void *params)
{
  while (true)
  {
    xEventGroupSetBits(evGrp, gotHttp);
    printf("got http\n");
    vTaskDelay(2000/portTICK_PERIOD_MS);
  }
}

void listenForBluetooth(void *params)
{
  while (true)
  {
    xEventGroupSetBits(evGrp, gotBLE);
    printf("got BLE\n");
    vTaskDelay(4000/portTICK_PERIOD_MS);
  }
}

void task1(void *params)
{
  while (true)
  {
    xEventGroupWaitBits(evGrp, gotHttp | gotBLE, true, true, portMAX_DELAY);
    printf("received http and BLE\n");
  }
}

void app_main(void)
{
  evGrp = xEventGroupCreate();
  xTaskCreate(&listenForHTTP, "get http", 2048, NULL, 1, NULL);
  xTaskCreate(&listenForBluetooth, "get BLE", 2048, NULL, 1, NULL);
  xTaskCreate(&task1, "do something http", 2048, NULL, 1, NULL);
}
