#include <stdio.h>
#include "nvs_flash.h"
#include "wifi_connect.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void)
{
  nvs_flash_init();
  wifi_connect_init();
  
  /** Connect to AP and then Station, and then back to AP and then station */
  /** Esp32 doesn't like too much flipping from Station to Access point in that case you need reset */
  while (true)
  {
    wifi_connect_ap("myEsp32AP", "password");  // password must be at least 8 characters.
    vTaskDelay(pdMS_TO_TICKS(10000));
    wifi_disconnect();

    wifi_connect_sta("myWiFi", "password", 10000);  // milliseconds
    vTaskDelay(pdMS_TO_TICKS(20000));
    wifi_disconnect();
  }
  
}
