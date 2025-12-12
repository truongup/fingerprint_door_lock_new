#ifndef  WIFI_APP_H
#define  WIFI_APP_H

#include <Arduino.h>
#include <WiFi.h>

#include <string.h>

#include "memory_struct.h"

void wifi_app_init(void);
void wifi_broadcast_for_setup(void);

#endif