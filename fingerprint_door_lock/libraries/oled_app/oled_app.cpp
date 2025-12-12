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
void oled_idle(void) {
    oled_sh110x_clear();
    oled_wifi_connected();
    oled_sh110x_display();
}

// UI FINGER
void oled_finger_start(void) {
    oled_sh110x_clear();
    oled_draw_image(0, 34, FinPr_start_bits ,FinPr_start_width, FinPr_start_height, SH110X_INVERSE);
    oled_sh110x_display();
}

void oled_finger_invalid(void) {
    oled_sh110x_clear();
    oled_draw_image(0, 34, FinPr_invalid_bits ,FinPr_invalid_width, FinPr_invalid_height, SH110X_INVERSE);
    oled_sh110x_display();
}

void oled_finger_valid(void) {
    oled_sh110x_clear();
    oled_draw_image(0, 54, FinPr_valid_bits ,FinPr_valid_width, FinPr_valid_height, SH110X_INVERSE);
    oled_sh110x_display();
}

void oled_finger_scan(void) {
    oled_sh110x_clear();
    oled_draw_image(5, 60, FinPr_scan_bits ,FinPr_scan_width, FinPr_scan_height, SH110X_INVERSE);
    oled_sh110x_display();
}

void oled_finger_fail(void) {
    oled_sh110x_clear();
    oled_draw_image(0, 74, FinPr_failed_bits ,FinPr_failed_width, FinPr_failed_height, SH110X_INVERSE);
    oled_sh110x_display();
}