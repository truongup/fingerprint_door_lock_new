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

void oled_sh110x_text(String content, uint8_t sz, uint8_t x, uint8_t y) {
    display.setTextSize(sz);             // Normal 1:1 pixel scale
    display.setTextColor(1);            // Draw white text
    display.setCursor(x, y);             // Start at top-left corner
    display.print(F(content.c_str()));
    display.display();
}

void oled_sh110x_process(int8_t progress, int8_t x, int8_t y, int8_t height) {
  display.clearDisplay();

  int barWidth = OLED_SCREEN_WIDTH - 2 * x;

  // Vẽ khung thanh tiến trình
  display.drawRect(x, y, barWidth, height, SH110X_WHITE);

  // Tính toán chiều rộng phần đã hoàn thành
  int filledWidth = map(progress, 0, 100, 0, barWidth - 2); // Trừ 2 để trừ viền

  // Vẽ phần đã hoàn thành
  display.fillRect(x + 1, y + 1, filledWidth, height - 2, SH110X_WHITE);

  // Hiển thị phần trăm tiến trình
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(OLED_SCREEN_WIDTH / 2 - (String(progress).length() * 6), y + height + 5); // Ước tính chiều rộng chữ
  display.print(progress);
  display.print("%");
  display.display();
}

void oled_monochrome_final(const char* statusText) {
//   display.clearDisplay();
  display.setTextColor(SH110X_WHITE);

  // Title
  display.setTextSize(1);
  display.setCursor(20, 0);
  display.print("ACCESS CONTROL");
  display.drawLine(0, 10, 127, 10, SH110X_WHITE);

  // Options
  display.setCursor(0, 16);
  display.print("1) Finger: Place fing");
  display.setCursor(0, 28);
  display.print("2) RFID  : Tap card");

  // Status box
  display.drawRoundRect(0, 46, 128, 18, 3, SH110X_WHITE);
  display.setCursor(4, 52);
  display.print("Status: ");
  display.print(statusText);

  display.display();
}

void oled_sh110x_rfid_valid(void) {
    display.setTextSize(1);
    display.setCursor(50, 0);
    display.print("RFID");

    display.setTextSize(2);
    display.setCursor(10, 14);
    display.print("SUCCESS");

    // Dấu tick (đẩy lên trên)
    display.drawLine(20, 34, 30, 44, SH110X_WHITE);
    display.drawLine(30, 44, 50, 30, SH110X_WHITE);

    // Thẻ RFID (đẩy lên theo)
    display.drawRoundRect(70, 32, 36, 20, 3, SH110X_WHITE);
    display.drawRect(73, 36, 10, 4, SH110X_WHITE);

    // Sóng RFID
    display.drawLine(108, 36, 112, 34, SH110X_WHITE);
    display.drawLine(108, 42, 114, 40, SH110X_WHITE);
    display.drawLine(108, 48, 116, 46, SH110X_WHITE);

    // ACCESS GRANTED (sát đáy, không đụng)
    display.setTextSize(1);
    display.setCursor(20, 56);
    display.print("ACCESS GRANTED");

    display.display();
}

void oled_sh110x_rfid_invalid(void) {
    // RFID
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(50, 0);
    display.print("RFID");

    // FAILED
    display.setTextSize(2);
    display.setCursor(22, 14);
    display.print("FAILED");

    // Dấu X (đẩy lên trên)
    display.drawLine(20, 30, 48, 50, SH110X_WHITE);
    display.drawLine(48, 30, 20, 50, SH110X_WHITE);

    // Thẻ RFID
    display.drawRoundRect(70, 32, 36, 20, 3, SH110X_WHITE);
    display.drawRect(73, 36, 10, 4, SH110X_WHITE); // chip

    // Sóng RFID
    display.drawLine(108, 36, 112, 34, SH110X_WHITE);
    display.drawLine(108, 42, 114, 40, SH110X_WHITE);
    display.drawLine(108, 48, 116, 46, SH110X_WHITE);

    // ACCESS DENIED (đẩy xuống đáy)
    display.setTextSize(1);
    display.setCursor(18, 56);
    display.print("ACCESS DENIED");
    display.display();
}

void oled_sh110x_admin_open_door(void) {
    // Tiêu đề
    display.setTextSize(1);
    display.setCursor(20, 0);
    display.println("ADMIN ACCESS");

    display.drawLine(0, 10, 127, 10, SH110X_WHITE);

    // Nội dung chính
    display.setTextSize(1);
    display.setCursor(15, 28);
    display.println("ADMIN DANG MO CUA");

    // Nhấn mạnh
    display.setCursor(32, 46);
    display.println(">>> OPEN <<<");

    display.display();
}