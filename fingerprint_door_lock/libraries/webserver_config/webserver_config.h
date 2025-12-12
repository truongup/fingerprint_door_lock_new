#ifndef WEBSERVER_CONFIG_H
#define WEBSERVER_CONFIG_H

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
// #include <LittleFS.h>
#include "SPIFFS.h"

#include "memory_struct.h"
#include "webserver_api.h"

extern AsyncWebServer *server;

void webserver_config_init(void);

static void webserver_source_init(void);
static void webserver_api_init(void);
#endif