#include "memory_struct.h"

/*  =========================== INFO USER ===========================    */
interface_data_user_t info_user;

operation_mode_t operation_mode = ILDE;

/*  =========================== BUTTON ===========================    */
button_debounce_t button_rst {
  .button_pin = GPIO_BUTTON_RESET_CONFIG,            // IO.0
  .button_active_state  = HIGH_TO_LOW,
  .button_time_debounce = 50,  // 50ms
};

/*  =========================== FIREBASE PATH ===========================    */
firebase_path_t firebase_path;

/*  =========================== UI ===========================    */
QueueHandle_t uiEventQueue;
long long     ui_timeout;
uint8_t       ui_active_event = UI_EVENT_INACTIVE;

/*  =========================== FLAG BLOCK ===========================    */
flag_block_t flag_block;