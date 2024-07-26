We are going to utilise the partition table to show how we can use not the default nvs but our own one and that gives us flexibility in terms of how big we can have this partition and how much data we want to store in our flash for nvs    
     
Go to `esp/esp-idf/components/partition_table` and copy the file `partitions_singleapp.csv` into our project and rename it to `paritions.csv` which contains the following table.   

| Name | Type | SubType | Offset | Size | Flags |
|:--------|:--------|:--------|:--------|:--------|:--------|
| nvs   | data  | nvs   |   | 0x6000  |  |
| phy_init   | data  | phy   |   | 0x1000  |  |
| factory   | app  | factory   |   | 1M  |  |     

The size of _nvs_ is currently sitting at 0x6000 which is 24,576/1024 24Kb by default. However if we need more we can create another column and pick a suitable name `MyNvs` and give it a 1M size.     

| Name | Type | SubType | Offset | Size | Flags |
|:--------|:--------|:--------|:--------|:--------|:--------|
| nvs   | data  | nvs   |   | 0x6000  |  |
| phy_init   | data  | phy   |   | 0x1000  |  |
| factory   | app  | factory   |   | 1M  |  |      
| MyNvs   | data  | nvs   |   | 1M  |  |      

Now configure this with `idf.py menuconfig` by going to `Partition table` > `Partition Table (...)` > `Custom parition table CSV` > `Custom parition CSV file` and give name of your parition table csv in our case its `partitions.csv` and hit Enter.    
Now go back and select `Serial flasher config --->` and increase `Flash size (2MB) --->` to 4MB and save `S` and exit    

We can change the API's little from `nvs_flash_init()` to `nvs_flash_init_partition()` and similarly `nvs_open_from_partition()`     

```c
#include <stdio.h>

#include "esp_log.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

#define TAG          "NVS"

void app_main(void)
{
  vTaskDelay(1000/portTICK_PERIOD_MS);
  ESP_ERROR_CHECK(nvs_flash_init_partition("MyNvs"));

  nvs_handle handle;

  /* Open from your partition `MyNvs` */
  ESP_ERROR_CHECK(nvs_open_from_partition("MyNvs", "store", NVS_READWRITE, &handle));

  nvs_stats_t nvsStats;
  nvs_get_stats("MyNvs", &nvsStats);
  ESP_LOGI(TAG, "used: %d, free: %d, total: %d, namespace count: %d", nvsStats.used_entries,
    nvsStats.free_entries, nvsStats.total_entries, nvsStats.namespace_count);
    
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
```                             