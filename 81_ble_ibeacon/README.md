### Steps to use bluetooth     
    
- `idf.py menuconfig` > Component config > Bluetooth > Host > NimBLE - BLE only      


### Boilerplate   
Typical boilerplate code you may use for nearly all the NimBLE application development      
```c
#include <stdio.h>

#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_nimble_hci.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h"
#include "services/gap/ble_svc_gap.h"

void host_task(void *param);
void ble_app_on_sync(void);

void app_main(void)
{
  nvs_flash_init();

  esp_nimble_hci_init();
  nimble_port_init();
  ble_hs_cfg.sync_cb = ble_app_on_sync;
  nimble_port_freertos_init(host_task);
}

void host_task(void *param)
{
  nimble_port_run();
}

void ble_app_on_sync(void)
{
  
}
```

