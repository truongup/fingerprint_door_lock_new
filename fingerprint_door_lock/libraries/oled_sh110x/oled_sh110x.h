#ifndef OLED_CORE_SH110x_H
#define OLED_CORE_SH110x_H

// Thư viện kết nối màn hình oled (Lưu ý thư viện SSD1306 không dùng được)
// Lỗi thư viện SSD1306: https://stackoverflow.com/questions/73163524/why-dose-my-oled-display-dont-work-with-esp32

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "oled_template.h"
#include "cfg_hw.h"

static Adafruit_SH1106G display = Adafruit_SH1106G(OLED_SCREEN_WIDTH, OLED_SCREEN_HEIGHT, &Wire, OLED_SCREEN_RST_PIN);

int oled_sh110x_init(void);

// kích cỡ chữ, màu chữ, hàng, cột, nội dung cần in ra
void oled_sh110x_set(uint8_t text_size, uint8_t text_color, uint8_t row, uint8_t col, const char *content);  

void oled_draw_image(uint8_t row, uint8_t col, const uint8_t *img, uint8_t size_img_width, uint8_t size_img_height, uint8_t img_color);

void oled_sh110x_display(void);
void oled_sh110x_clear(void);

// text
void oled_sh110x_text(String content, uint8_t sz, uint8_t x, uint8_t y);

// process bar
void oled_sh110x_process(int8_t progress, int8_t x, int8_t y, int8_t height);

// monochrome
// void oled_monochrome_draw_ui(const char* statusText);
// void oled_monochrome_draw_ui_128x64(const char* statusText);
void oled_monochrome_final(const char* statusText);

// rfid
void oled_sh110x_rfid_valid(void);
void oled_sh110x_rfid_invalid(void);

// admin open door
void oled_sh110x_admin_open_door(void);
#endif