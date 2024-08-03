#include <stdio.h>
#include <time.h>
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "protocol_examples_common.h"
#include "esp_log.h"
#include "esp_sntp.h"

#define TAG "NTP TIME"

SemaphoreHandle_t got_time_semaphore;

void print_time()
{
  time_t now =0;
  time(&now); // milliseconds from Jan 1970

  struct tm *time_info = localtime(&now);

  char time_buf[50];
  strftime(time_buf, sizeof(time_buf), "%c", time_info);
  ESP_LOGI(TAG, "******** %s ********", time_buf);

}

void on_got_time(struct timeval *tv)
{
  printf("on got callback %lld\n", tv->tv_sec);
  print_time();

  xSemaphoreGive(got_time_semaphore);
}

void app_main(void)
{
  got_time_semaphore = xSemaphoreCreateBinary();

  setenv("TZ", "GMT+0IST-1.M3.5.0/01:00:00.M10.5.0/02:00:00", 1);   // second argument `opnode time-zones` 
  tzset();

  printf("first time print\n");
  print_time();

  nvs_flash_init();
  esp_netif_init();
  esp_event_loop_create_default();
  example_connect();

  esp_sntp_init();
  esp_sntp_set_sync_mode(SNTP_SYNC_MODE_IMMED);
  esp_sntp_setservername(0, "pool.ntp.org");
  esp_sntp_set_time_sync_notification_cb(on_got_time);

  xSemaphoreTake(got_time_semaphore, portMAX_DELAY);

  for (int i =0; i< 5; ++i)
  {
    print_time();
    vTaskDelay(pdMS_TO_TICKS(1000));
  }

  esp_restart();
}
