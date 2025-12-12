#ifndef GPIO_APP_H
#define GPIO_APP_H

#include <Arduino.h>
#include <cfg_hw.h>
#include <cfg_sw.h>

void gpio_app_init(void);
void gpio_led_set(uint8_t gpio_pin, uint8_t state);
void gpio_solenoid_set(uint8_t state);
void gpio_buzzer_set(uint8_t state);

#endif