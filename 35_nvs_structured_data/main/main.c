#include <stdio.h>

#include "esp_log.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

#define TAG          "NVS"  
   
typedef struct
{
  char name[20];
  int age;
  int id;
} Cat;  

void app_main(void)
{
  vTaskDelay(1000/portTICK_PERIOD_MS);
  ESP_ERROR_CHECK(nvs_flash_init_partition("MyNvs"));

  nvs_handle handle;

  /* Open from your partition `MyNvs` */
  ESP_ERROR_CHECK(nvs_open_from_partition("MyNvs", "cat_store", NVS_READWRITE, &handle));

  nvs_stats_t nvsStats;
  nvs_get_stats("MyNvs", &nvsStats);
  ESP_LOGI(TAG, "used: %d, free: %d, total: %d, namespace count: %d", nvsStats.used_entries,
    nvsStats.free_entries, nvsStats.total_entries, nvsStats.namespace_count);

  /* Read purr 😺😺😺😺😺 */
  char catKey[15];
  Cat cat;
  size_t catSize;
  for (int i=0; i< 5; ++i)
  {
    sprintf(catKey, "cat_%d", i);
    esp_err_t result = nvs_get_blob(handle, catKey, (void*)&cat, &catSize);
    switch (result)
    {
      case ESP_ERR_NVS_NOT_FOUND: 
        ESP_LOGE(TAG, "Value not set yet");
        break;
      case ESP_OK:
        ESP_LOGI(TAG, "cat nanme: %s, age %d, id %d", cat.name, cat.age, cat.id);
        break;
      default:
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(result));
        break;
    }
  }  
  
  /* Write */
  for (int i=0; i< 5; ++i)
  {
    sprintf(catKey, "cat_%d", i);
    Cat newCat;
    sprintf(newCat.name, "Mr 😺 %d", i);
    newCat.age = i+2;
    newCat.id = i;
    ESP_ERROR_CHECK(nvs_set_blob(handle, catKey, (void*)&newCat, sizeof(Cat)));
    ESP_ERROR_CHECK(nvs_commit(handle));  
  }
  nvs_close(handle);

}
