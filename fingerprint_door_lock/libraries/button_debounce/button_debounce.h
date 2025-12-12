#ifndef BUTTON_DEBOUNCE_H
#define BUTTON_DEBOUNCE_H

#include "Arduino.h"

#define TIME_SHORT_PRESS    500
#define TIME_LONG_PRESS     3000

#define IS_NON_PRESS_FLAG   0
#define IS_PRESS_FLAG       1

// trạng thái khi nhấn nút
typedef enum {
    HIGH_TO_LOW,            // đối với trở kéo lên
    LOW_TO_HIGH             // đối với trở kéo xuống
} button_active_t;


// trạng thái hiện tại của nút nhấn
typedef enum {
    LOW_STATE,
    HIGH_STATE
} button_state_t;

// định nghĩa các sự kiện của nút nhấn
typedef enum {
    BUTTON_EVENT_PRESS,
    BUTTON_EVENT_RELEASE,
    BUTTON_EVENT_SHORT_PRESS,
    BUTTON_EVENT_LONG_PRESS
} button_event_t;

typedef struct {
    uint8_t button_id;
    button_event_t button_event;
} button_msg_t;

typedef struct {
    uint8_t             button_pin;
    
    button_state_t      button_curr_state;          // lưu trạng thái hiện tại của nút nhấn
    button_state_t      button_filter_state;        // lưu trạng thái filter 
    button_state_t      button_last_state;  

    uint8_t             button_is_press;            // cờ khi nhấn ntus

    button_active_t     button_active_state;        // trạng thái nhấn nút, ví dụ cho nút nhấn kéo lên xuống

    uint16_t            button_time_debounce;       // thời gian chống dội, ví dụ 20ms
    uint16_t            button_time_press;          // thời gian khi vừa nhấn nút (để xử lý chống dội)

    uint32_t            button_time_start_press;
    uint32_t            button_time_end_press;

    uint8_t             button_press_timeout_flag;
} button_debounce_t;

extern QueueHandle_t buttonQueue;

void button_init(button_debounce_t *btn);
void button_handle(button_debounce_t *btn);

static bool button_read_state(uint8_t pin);
static bool button_config(button_debounce_t **btn, button_active_t act_sta);
#endif