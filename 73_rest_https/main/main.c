#include <stdio.h>

#include "wifi_conn.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "nvs_flash.h"

static const char *TAG = "REST";
extern const uint8_t cert[] asm("_binary_amazon_single_cert_pem_start");

typedef struct 
{
  uint8_t *buffer;
  int buffer_index;
} chunk_payload_t;

void fetch_quote();
esp_err_t onClientData(esp_http_client_event_t *evt);

void app_main(void)
{
  ESP_ERROR_CHECK(nvs_flash_init());
  wifi_init();
  ESP_ERROR_CHECK(wifi_connect_sta("[YOUR WIFI NAME]", "[YOUR WIFI PASSWORD]", 10000));
  
  fetch_quote();
  vTaskDelay(pdMS_TO_TICKS(3000));  
}

void fetch_quote()
{
  chunk_payload_t chunk_payload = {0};
  esp_http_client_config_t esp_http_client_config = {
    .url = "https://weatherapi-com.p.rapidapi.com/forecast.json?q=Carrick-on-suir&days=3",
    .method = HTTP_METHOD_GET,
    .event_handler = onClientData,
    .user_data = &chunk_payload,
    .cert_pem = (char*)cert,
  };
  esp_http_client_handle_t client = esp_http_client_init(&esp_http_client_config);
  esp_http_client_set_header(client, "Content-Type", "application/json");
  esp_http_client_set_header(client, "x-rapidapi-key", "[YOUR API KEYS HERE]");
  esp_http_client_set_header(client, "x-rapidapi-host", "weatherapi-com.p.rapidapi.com");
  esp_err_t err = esp_http_client_perform(client);
  
  if(err == ESP_OK)
  {
    ESP_LOGI(TAG, "HTTP GET status = %d, result = %s\n",
      esp_http_client_get_status_code(client),
      chunk_payload.buffer);
  }
  else
  {
    ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
  }
  if (chunk_payload.buffer != NULL)
  {
    free(chunk_payload.buffer);
  }
  esp_http_client_cleanup(client);
  wifi_disconnect();
}

esp_err_t onClientData(esp_http_client_event_t *evt)
{
  switch(evt->event_id)
  {
    case HTTP_EVENT_ON_DATA:
    {
      // ESP_LOGI(TAG, "Length=%d", evt->data_len);
      // printf("%.*s\n", evt->data_len, (char *)evt->data);
      chunk_payload_t *chunk_payload = evt->user_data;
      chunk_payload->buffer = realloc(chunk_payload->buffer, chunk_payload->buffer_index + evt->data_len + 1);
      memcpy(&chunk_payload->buffer[chunk_payload->buffer_index], (uint8_t*)evt->data, evt->data_len);
      chunk_payload->buffer_index = chunk_payload->buffer_index + evt->data_len;
      chunk_payload->buffer[chunk_payload->buffer_index] = 0;
      // printf("buffer******* %s\n", chunk_payload->buffer);
    }
      break;
    default:
    break;  
  }
  return ESP_OK;
}