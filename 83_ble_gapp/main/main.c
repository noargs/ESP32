#include <stdio.h>

#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_nimble_hci.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h"
#include "services/gap/ble_svc_gap.h"

#define DEVICE_NAME "Ibn📍"

uint8_t ble_addr_type;

void ble_app_advertise(void);
void host_task(void *param);
void ble_app_on_sync(void);

static int ble_gap_event(struct ble_gap_event *event, void *arg);

void app_main(void)
{
  nvs_flash_init();

  esp_nimble_hci_init();
  nimble_port_init();

  ble_svc_gap_device_name_set(DEVICE_NAME);
  ble_svc_gap_init();

  ble_hs_cfg.sync_cb = ble_app_on_sync;
  nimble_port_freertos_init(host_task);
}

void host_task(void *param)
{
  nimble_port_run();
}

void ble_app_on_sync(void)
{
  ble_hs_id_infer_auto(0, &ble_addr_type);
  ble_app_advertise();
}

void ble_app_advertise(void)
{
  struct ble_hs_adv_fields fields;
  memset(&fields, 0, sizeof(fields));

  fields.flags = BLE_HS_ADV_F_DISC_GEN | BLE_HS_ADV_F_DISC_LTD;
  fields.tx_pwr_lvl_is_present = 1;
  fields.tx_pwr_lvl = BLE_HS_ADV_TX_PWR_LVL_AUTO;

  fields.name = (uint8_t*)ble_svc_gap_device_name();
  fields.name_len = strlen(ble_svc_gap_device_name());
  fields.name_is_complete = 1;

  ble_gap_adv_set_fields(&fields);

  struct ble_gap_adv_params adv_params;
  memset(&adv_params, 0, sizeof(adv_params));
  adv_params.conn_mode = BLE_GAP_CONN_MODE_UND;
  adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN;

  ble_gap_adv_start(ble_addr_type, NULL, BLE_HS_FOREVER, &adv_params, ble_gap_event, NULL);
}

static int ble_gap_event(struct ble_gap_event *event, void *arg)
{
  switch(event->type)
  {
    case BLE_GAP_EVENT_CONNECT:
      ESP_LOGI("GAP", "BLE_GAP_EVENT_CONNECT %s", event->connect.status == 0 ? "OK":"FAILED");
      if (event->connect.status != 0)
      {
        ble_app_advertise();
      }
      break;
    case BLE_GAP_EVENT_DISCONNECT:
      ESP_LOGI("GAP", "BLE_GAP_EVENT_DISCONNECT");
      ble_app_advertise();
      break;
    case BLE_GAP_EVENT_ADV_COMPLETE:
      ESP_LOGI("GAP", "BLE_GAP_EVENT_ADV_COMPLETE");
      ble_app_advertise();
      break;
    case BLE_GAP_EVENT_SUBSCRIBE:
      ESP_LOGI("GAP", "BLE_GAP_EVENT_SUBSCRIBE");
      break;
    default:
      break;  
  }
  return 0;
}
