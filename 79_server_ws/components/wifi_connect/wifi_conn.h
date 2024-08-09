#ifndef WIFI_CONN_H_
#define WIFI_CONN_H_

#include "esp_wifi.h"

void wifi_init(void);
esp_err_t wifi_connect_sta(char *ssid, char *password, int timeout);
void wifi_connect_ap(const char *ssid, const char *pass);
void wifi_disconnect(void);

#endif