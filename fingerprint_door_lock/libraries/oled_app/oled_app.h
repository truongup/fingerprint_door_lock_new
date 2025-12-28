#ifndef OLED_APP_H
#define OLED_APP_H

#include "oled_sh110x.h"
#include "oled_template.h"

#define OLED_CLEAR 1

int oled_app_init(void);
void oled_clear(void);

// display wifi
void oled_wifi_start(const char *wifi_ssid);
void oled_wifi_connected(void);

// idle
void oled_idle(uint8_t clear = 0);

// print text
void oled_text(String content, uint8_t sz, uint8_t x, uint8_t y);

// display finger
void oled_finger_start(uint8_t clear = 0);
void oled_finger_invalid(uint8_t clear = 0);
void oled_finger_valid(uint8_t clear = 0);
void oled_finger_fail(uint8_t clear = 0);
void oled_finger_scan(uint8_t clear = 0);

// display rfid
void oled_rfid_scan(uint8_t clear = 0);
void oled_rfid_valid(uint8_t clear = 0);
void oled_rfid_invalid(uint8_t clear = 0);

// process wait
void oled_add_user_wait_process(uint8_t clear = 0);

// admin
void oled_admin_open_door(uint8_t clear = 0);

#endif