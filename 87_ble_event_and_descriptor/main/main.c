#include <stdio.h>

#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_nimble_hci.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"

#define DEVICE_NAME "Ibn📍"

uint8_t ble_addr_type;

void ble_app_advertise(void);
void host_task(void *param);
void ble_app_on_sync(void);
static int ble_gap_event(struct ble_gap_event *event, void *arg);
static int device_info(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg);
static int device_write(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg);
static int battery_read(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg);
void update_battery_timer();

static TimerHandle_t timer_handler;
uint16_t battery_characteristics_att_handler;

#define DEVICE_INFO_SERVICE 0x180A       // https://www.bluetooth.com/wp-content/uploads/Files/Specification/Assigned_Numbers.html
#define MANUFACTURER_NAME   0x2A29

#define BATTERY_SERVICE     0x180F
#define BATTERY_LEVEL_CHAR  0x2A19

uint8_t battery_level = 100;

static const struct ble_gatt_svc_def gat_svcs[] = {
  {.type = BLE_GATT_SVC_TYPE_PRIMARY,
   .uuid = BLE_UUID16_DECLARE(DEVICE_INFO_SERVICE),
   .characteristics = (struct ble_gatt_chr_def[]){
      {.uuid = BLE_UUID16_DECLARE(MANUFACTURER_NAME),
       .flags = BLE_GATT_CHR_F_READ,
       .access_cb = device_info},
      {.uuid = BLE_UUID128_DECLARE(0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff),
       .flags = BLE_GATT_CHR_F_WRITE,
       .access_cb = device_write}, 
      {0}}},
  {.type = BLE_GATT_SVC_TYPE_PRIMARY,
  .uuid = BLE_UUID16_DECLARE(BATTERY_SERVICE),
  .characteristics = (struct ble_gatt_chr_def[]){
    {.uuid = BLE_UUID16_DECLARE(BATTERY_LEVEL_CHAR),
      .flags = BLE_GATT_CHR_F_READ,
      .val_handle = &battery_characteristics_att_handler,
      .access_cb = battery_read},
    {0}}},    
  {0}};

void app_main(void)
{
  nvs_flash_init();

  esp_nimble_hci_init();
  nimble_port_init();

  ble_svc_gap_device_name_set(DEVICE_NAME);
  ble_svc_gap_init();
  ble_svc_gatt_init();

  ble_gatts_count_cfg(gat_svcs);
  ble_gatts_add_svcs(gat_svcs);

  timer_handler = xTimerCreate("update_battery_timer", pdMS_TO_TICKS(1000), pdTRUE, NULL, update_battery_timer);

  ble_hs_cfg.sync_cb = ble_app_on_sync;
  nimble_port_freertos_init(host_task);
}

void host_task(void *param)
{
  nimble_port_run();
}

void update_battery_timer()
{
  if(battery_level-- == 0)
  {
    battery_level = 100;
  }
  printf("notify battery level is %d\n", battery_level);
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
      if (event->subscribe.attr_handle == battery_characteristics_att_handler)
      {
        xTimerStart(timer_handler, 0);
      }
      break;
    default:
      break;  
  }
  return 0;
}

static int device_info(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
  os_mbuf_append(ctxt->om, "manufacturer name", strlen("manufacturer name"));
  return 0;
}

static int device_write(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
  printf("incoming message: %.*s\n", ctxt->om->om_len, ctxt->om->om_data);
  return 0;
}

static int battery_read(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
  uint8_t battery_level = 85;
  os_mbuf_append(ctxt->om, &battery_level, sizeof(battery_level));
  return 0;
}
