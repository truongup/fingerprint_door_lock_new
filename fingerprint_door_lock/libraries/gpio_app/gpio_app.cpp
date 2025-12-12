#include <gpio_app.h>

void gpio_app_init(void) {
    pinMode(GPIO_DRIVE_SOLENOID,    OUTPUT);
    pinMode(GPIO_DRIVE_LED_GREEN,   OUTPUT);
    pinMode(GPIO_DRIVE_LED_RED,     OUTPUT);
    pinMode(GPIO_DRIVE_BUZZER,      OUTPUT);

    // enable led red
    // gpio_led_set(GPIO_DRIVE_LED_RED, LED_ON);

    log_d("GPIO init");
}

void gpio_led_set(uint8_t gpio_pin, uint8_t state) {
    digitalWrite(gpio_pin, state);
}

void gpio_solenoid_set(uint8_t state) {
    digitalWrite(GPIO_DRIVE_SOLENOID, state);
}

void gpio_buzzer_set(uint8_t state) {
    if(state == BUZZER_ON) {
        tone(GPIO_DRIVE_BUZZER, 2000);
        return;
    }
        
    noTone(GPIO_DRIVE_BUZZER);
}