#include "oled_sh110x.h"

int oled_sh110x_init(void) {
    if(!display.begin(OLED_SCREEN_ADDRESS, true)) {
        log_d("Did not find oled display :(\n"); 
        return -1;
    }

    display.display();
    display.clearDisplay();
    log_d("Found oled display!\n");
    return 0;
}

void oled_sh110x_clear(void) {
    display.clearDisplay();
}

void oled_sh110x_display(void) {
    display.display();
}

void oled_sh110x_set(uint8_t text_size, uint8_t text_color, uint8_t row, uint8_t col, const char *content) {
    display.setTextSize(text_size);
    display.setTextColor(text_color); 
    display.setCursor(col, row);     
    display.print(content);    
}

void oled_draw_image(uint8_t row, uint8_t col, const uint8_t *img, uint8_t size_img_width, uint8_t size_img_height, uint8_t img_color) {
    display.drawBitmap( col, row, img, size_img_width, size_img_height, img_color);
}