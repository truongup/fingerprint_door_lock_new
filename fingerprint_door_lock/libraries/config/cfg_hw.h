#ifndef CONFIG_HARDWARE_H
#define CONFIG_HARDWARE_H

#include "Arduino.h"

/*  Fingerprint AS608: Cấu hình serial port và chân tx rx giao tiếp cảm biến vân tay AS608  */
#define FINGER_PRINT_HARDWARE_SERIAL (1) //serial1
#define FINGER_PRINT_BAUDRATE   57600
#define FINGER_PRINT_RX_PIN     12 
#define FINGER_PRINT_TX_PIN     13

/**
 *  AS608     ESP32
 * -----------------
 *  VCC:      3.3V 
 *  GND:      GND
 *  TX:       12
 *  RX:       13 
*/

/*  OLED: Cấu hình I2C giao tiếp màn hình OLED  */
#define OLED_SCREEN_ADDRESS     0x3C
#define OLED_SCREEN_WIDTH       128
#define OLED_SCREEN_HEIGHT      64
#define OLED_SCREEN_SCL_PIN     22
#define OLED_SCREEN_SDA_PIN     21
#define OLED_SCREEN_RST_PIN     -1       // Chân reset, -1 tức là dùng chung chân reset với esp32

/**
 * |    OLED     |       ESP32   |
 * |-----------------------------|
 * |    VCC:     |       3.3V    |
 * |    GND:     |       GND     |
 * |    SCL:     |       22      |
 * |    SDA:     |       21      |
*/

/*  RFID: Cấu hình SPI giao tiếp RFID   */
#define RFID_SPI_SS_PIN         (5)
#define RFID_SPI_RST_PIN        (4)

/*  GPIO: Cấu hình chân INPUT OUTPUT*/
#define GPIO_BUTTON_CONFIG           
#define GPIO_BUTTON_RESET_CONFIG    0   // tạm thời để IO.0     
#define GPIO_DRIVE_LED_GREEN        33
#define GPIO_DRIVE_LED_RED          32
#define GPIO_DRIVE_SOLENOID         27
#define GPIO_DRIVE_BUZZER           25
#endif