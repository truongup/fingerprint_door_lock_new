#ifndef OLED_APP_H
#define OLED_APP_H

#include "oled_sh110x.h"
#include "oled_template.h"

int oled_app_init(void);
void oled_clear(void);

// display wifi
void oled_wifi_start(const char *wifi_ssid);
void oled_wifi_connected(void);

// idle
void oled_idle(void);

// display finger
void oled_finger_start(void);
void oled_finger_invalid(void);
void oled_finger_valid(void);
void oled_finger_fail(void);
void oled_finger_scan(void);
#endif