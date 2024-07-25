#include <stdio.h>
#include <string.h>

#include "esp_log.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

#define TAG          "NVS"

void app_main(void)
{
  //vTaskDelay(1000/portTICK_PERIOD_MS); // it will give a chance  for the `monitor` to actually reset and kick in

  ESP_ERROR_CHECK(nvs_flash_init());

  nvs_handle handle;

  /* Open */
  ESP_ERROR_CHECK(nvs_open("store", NVS_READWRITE, &handle));

  /* Read */
  int32_t val = 0;
  esp_err_t result = nvs_get_i32(handle, "val", &val);
  switch (result)
  {
    case ESP_ERR_NVS_NOT_FOUND: 
      ESP_LOGE(TAG, "Value not set yet");
      break;
    case ESP_OK:
      ESP_LOGI(TAG, "Value is %d", (int)val);
      break;
    default:
      ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(result));
      break;
  }
  val++; // increase everytime chip resets,  `monitor` will also reset it as well as `Reset` button on the board

  /* Write */
  ESP_ERROR_CHECK(nvs_set_i32(handle, "val", val));

  ESP_ERROR_CHECK(nvs_commit(handle));
  nvs_close(handle);
}
