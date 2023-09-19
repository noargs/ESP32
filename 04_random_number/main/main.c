#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_random.h" // "esp_system.h" for idf < 4.x
#include "math.h"

#define TAG              "DICE"

int dice_role()
{
  int random = esp_random();
  int positive_number = abs(random);
  int dice_number = (positive_number % 6) + 1;
  return dice_number;
}

void app_main(void)
{
  while (1)
  {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    ESP_LOGI(TAG, "random number %d", dice_role());
  }
}
