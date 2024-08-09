#include <stdio.h>

#include "wifi_conn.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_http_server.h"
#include "mdns.h"

static const char *TAG = "SERVER";
static void init_server();
static esp_err_t on_default_url(httpd_req_t *req);
void start_mdns_server();

void app_main(void)
{
  ESP_ERROR_CHECK(nvs_flash_init());
  wifi_init();
  ESP_ERROR_CHECK(wifi_connect_sta("[YOUR WIFI NAME]", "[YOUR WIFI PASSWORD]", 10000));

  init_server();
  start_mdns_server();

}

void start_mdns_server()
{
  mdns_init();
  mdns_hostname_set("my-esp32");        // visit `my-esp32.local`
  mdns_instance_name_set("MDNS test");
}

static void init_server()
{
  httpd_handle_t server = NULL;
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();

  ESP_ERROR_CHECK(httpd_start(&server, &config));

  httpd_uri_t default_url = {
    .uri = "/",
    .method = HTTP_GET,
    .handler = on_default_url
  };
  httpd_register_uri_handler(server, &default_url);
}

static esp_err_t on_default_url(httpd_req_t *req)
{
  ESP_LOGI(TAG, "URL: %s", req->uri);
  httpd_resp_sendstr(req, "hello world");
  return ESP_OK;
}