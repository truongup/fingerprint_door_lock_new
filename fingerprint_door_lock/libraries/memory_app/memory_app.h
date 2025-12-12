#ifndef MEMORY_APP_H
#define MEMORY_APP_H

#include <stdlib.h> 

#include "Arduino.h"
#include <ArduinoJson.h>

#include "flash_read_write.h"    // sử dụng SPIFFS
// #include "flash_read_write_v2.h"        // _______ LittleFS

#include "memory_struct.h"
#include "cfg_sw.h"

#define FILE_CONFIG_WIFI         "/cfg_wifi.json"
#define FILE_CONFIG_IP           "/cfg_ip.json"
#define FILE_FIREBASE_CONFIG     "/cfg_firebase.json"

int memory_init(void);
int memory_flash_to_ram(void);

int change_info_wifi_station(char *new_ssid, char *new_pass);

#endif