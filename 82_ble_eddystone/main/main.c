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
  ble_addr_t addr;
  ble_hs_id_gen_rnd(1, &addr);
  ble_hs_id_set_rnd(addr.val);

  struct ble_hs_adv_fields fields = (struct ble_hs_adv_fields){0};
  ble_eddystone_set_adv_data_url(&fields, BLE_EDDYSTONE_URL_SCHEME_HTTP, "github", strlen("github"), BLE_EDDYSTONE_URL_SUFFIX_COM, -30);

  struct ble_gap_adv_params adv_params = (struct ble_gap_adv_params){0};
  ble_gap_adv_start(BLE_OWN_ADDR_RANDOM, NULL, BLE_HS_FOREVER, &adv_params, NULL, NULL);
}
