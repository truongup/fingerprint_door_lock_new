#ifndef WEBSERVER_API_H
#define WEBSERVER_API_H

#include "ArduinoJson.h"
#include <ESPAsyncWebServer.h>

#include "memory_struct.h"
#include "memory_app.h"

void api_get_request_device_info(AsyncWebServerRequest *request);

void api_post_request_wifi_config(AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total);

#endif