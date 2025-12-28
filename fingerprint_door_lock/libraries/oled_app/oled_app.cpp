#include "oled_app.h"

int oled_app_init(void) {
    return oled_sh110x_init();
}

void oled_clear(void) {
    oled_sh110x_clear();
}

// UI WIFI
void oled_wifi_start(const char *wifi_ssid) {
    oled_sh110x_clear();
    oled_sh110x_set(1, SH110X_WHITE, 0, 0, "Connecting to \n");

    const char *ssid = wifi_ssid;

    oled_sh110x_set(1, SH110X_WHITE, 50, 0, ssid);
    oled_draw_image(15, 50, Wifi_start_bits, Wifi_start_width, Wifi_start_height, SH110X_INVERSE);

    oled_sh110x_display();
}

void oled_wifi_connected(void) {
    oled_sh110x_clear();
    oled_draw_image(15, 33, Wifi_connected_bits ,Wifi_connected_width, Wifi_connected_height, SH110X_INVERSE);
    oled_sh110x_display();
}

// UI IDLE
void oled_idle(uint8_t clear) {
    if(clear == OLED_CLEAR)
        oled_sh110x_clear();
    
    oled_monochrome_final("Ready");
}

// UI FINGER
void oled_finger_start(uint8_t clear) {
    if(clear == OLED_CLEAR)
        oled_sh110x_clear();
    
    oled_draw_image(0, 34, FinPr_start_bits ,FinPr_start_width, FinPr_start_height, SH110X_INVERSE);
    oled_sh110x_display();
}

void oled_finger_invalid(uint8_t clear) {
    if(clear == OLED_CLEAR)
        oled_sh110x_clear();

    oled_draw_image(0, 34, FinPr_invalid_bits ,FinPr_invalid_width, FinPr_invalid_height, SH110X_INVERSE);
    oled_sh110x_display();
}

void oled_finger_valid(uint8_t clear) {
    if(clear == OLED_CLEAR)
        oled_sh110x_clear();

    oled_draw_image(0, 54, FinPr_valid_bits ,FinPr_valid_width, FinPr_valid_height, SH110X_INVERSE);
    oled_sh110x_display();
}

void oled_finger_scan(uint8_t clear) {
    if(clear == OLED_CLEAR)
        oled_sh110x_clear();

    oled_draw_image(5, 60, FinPr_scan_bits ,FinPr_scan_width, FinPr_scan_height, SH110X_INVERSE);
    oled_sh110x_display();
}

void oled_finger_fail(uint8_t clear) {
    if(clear == OLED_CLEAR)
        oled_sh110x_clear();

    oled_draw_image(0, 74, FinPr_failed_bits ,FinPr_failed_width, FinPr_failed_height, SH110X_INVERSE);
    oled_sh110x_display();
}

// Text
void oled_text(String content, uint8_t sz, uint8_t x, uint8_t y) {
    oled_sh110x_text(content, sz, x, y);
}

// RFID
void oled_rfid_scan(uint8_t clear) {
    if(clear == OLED_CLEAR)
        oled_sh110x_clear();
    oled_sh110x_text("RFID SCAN", 2, 5, 20);
}

void oled_rfid_valid(uint8_t clear) {
    if(clear == OLED_CLEAR)
        oled_sh110x_clear();
    oled_sh110x_rfid_valid();
}

void oled_rfid_invalid(uint8_t clear) {
    if(clear == OLED_CLEAR)
        oled_sh110x_clear();
    oled_sh110x_rfid_invalid();
}

// Wait Process
void oled_add_user_wait_process(uint8_t clear) {
    if(clear == OLED_CLEAR)
        oled_sh110x_clear();
    oled_sh110x_process(95, 10, 20, 10);
    oled_sh110x_text("Wating...", 1, 50, 50);
}

// ADMIN
void oled_admin_open_door(uint8_t clear) {
    if(clear == OLED_CLEAR)
        oled_sh110x_clear();
    
    oled_sh110x_admin_open_door();
}