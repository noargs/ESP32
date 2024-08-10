#include <stdio.h>

#include "wifi_conn.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_http_server.h"
#include "mdns.h"
#include "toggle_led.h"
#include "cJSON.h"
#include "push_button.h"
#include "ws_msg.h"

#define WS_MAX_SIZE 1024

static const char *TAG = "SERVER";
static int client_session_id; 
static httpd_handle_t server = NULL;

static void init_server();
static esp_err_t on_default_url(httpd_req_t *req);
static esp_err_t on_toggle_led_url(httpd_req_t *req);
static esp_err_t on_ws_url(httpd_req_t *req);
void start_mdns_server();

void app_main(void)
{
  ESP_ERROR_CHECK(nvs_flash_init());
  init_led();
  init_btn();
  wifi_init();
  ESP_ERROR_CHECK(wifi_connect_sta("__hw_wifi__", "dude@dude.com", 10000));

  start_mdns_server();
  init_server();
}

static void init_server()
{
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();

  ESP_ERROR_CHECK(httpd_start(&server, &config));

  httpd_uri_t default_url = {
    .uri = "/",
    .method = HTTP_GET,
    .handler = on_default_url
  };
  httpd_register_uri_handler(server, &default_url);

  httpd_uri_t toggle_led_url = {
  .uri = "/api/toggle-led",
  .method = HTTP_POST,
  .handler = on_toggle_led_url,
  };
  httpd_register_uri_handler(server, &toggle_led_url);

  httpd_uri_t web_socket_url = {
    .uri = "/ws",
    .method = HTTP_GET,
    .handler = on_ws_url,
    .is_websocket = true,
  };
  httpd_register_uri_handler(server, &web_socket_url);

} 

static esp_err_t on_default_url(httpd_req_t *req)
{
  ESP_LOGI(TAG, "URL: %s", req->uri);
  httpd_resp_sendstr(req, "hello world");
  return ESP_OK;
}

static esp_err_t on_toggle_led_url(httpd_req_t *req)
{
  char buffer[100];
  memset(&buffer, 0, sizeof(buffer));
  httpd_req_recv(req, buffer, req->content_len);
  cJSON *payload = cJSON_Parse(buffer);
  cJSON *is_on_json = cJSON_GetObjectItem(payload, "is_on");
  bool is_on = cJSON_IsTrue(is_on_json);
  cJSON_Delete(payload);
  toggle_led(is_on);
  httpd_resp_set_status(req, "204 NO CONTENT");
  httpd_resp_send(req, NULL, 0);
  return ESP_OK;
}

static esp_err_t on_ws_url(httpd_req_t *req)
{
  client_session_id = httpd_req_to_sockfd(req);
  if (req->method == HTTP_GET) return ESP_OK;

  httpd_ws_frame_t ws_packet;
  memset(&ws_packet, 0, sizeof(httpd_ws_frame_t));
  ws_packet.type = HTTPD_WS_TYPE_TEXT;
  ws_packet.payload = malloc(WS_MAX_SIZE);
  httpd_ws_recv_frame(req, &ws_packet, WS_MAX_SIZE);
  printf("ws payload: %.*s\n", ws_packet.len, ws_packet.payload);

  free(ws_packet.payload);

  char *response = "connected OK 😊";
  httpd_ws_frame_t ws_response = {
    .final = true,
    .fragmented = false,
    .type = HTTPD_WS_TYPE_TEXT,
    .payload = (uint8_t *)response,
    .len = strlen(response),
  };
  return httpd_ws_send_frame(req, &ws_response);
}

esp_err_t send_ws_message(char *message)
{
  if (!client_session_id)
  {
    ESP_LOGE(TAG, "no client_session_id");
    return -1;
  }
  httpd_ws_frame_t ws_message = {
    .final = true,
    .fragmented = false,
    .len = strlen(message),
    .payload = (uint8_t *)message,
    .type = HTTPD_WS_TYPE_TEXT,
  };
  return httpd_ws_send_frame_async(server, client_session_id, &ws_message);
}

void start_mdns_server()
{
  mdns_init();
  mdns_hostname_set("my-esp32");        // visit `my-esp32.local`
  mdns_instance_name_set("MDNS test");
}