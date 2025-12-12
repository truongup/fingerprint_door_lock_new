#include "button_debounce.h"

QueueHandle_t buttonQueue= NULL;


void button_init(button_debounce_t *btn) {
    if(buttonQueue == NULL) {
        buttonQueue = xQueueCreate(5, sizeof(button_msg_t)); 
        if(buttonQueue == NULL) {
            log_d("Queue create failed!");
        }
    }


    button_config(&btn, btn->button_active_state);
    pinMode(btn->button_pin, INPUT);         // trở kéo lên / xuống thì do phần cứng luôn
}

void button_handle(button_debounce_t *btn) {
    uint8_t button_pin = btn->button_pin;
    button_state_t read_state = (button_state_t)button_read_state(button_pin);

    // nếu trạng thái nút hiện tại khác trạng thái được lưu về trước đó
    if(read_state != btn->button_filter_state) {
        btn->button_time_press      = millis();
        btn->button_is_press        = IS_PRESS_FLAG;
        btn->button_filter_state    = read_state;
    }

    // nếu thời gian nhấn nút lớn hơn time debounce (ví dụ 20ms) thì tức là mình đang nhấn nút rồi
    if((millis() - btn->button_time_press) > (btn->button_time_debounce) && btn->button_is_press) {
        btn->button_curr_state    = btn->button_filter_state;
        btn->button_is_press      = IS_NON_PRESS_FLAG;
    }

    // nếu trạng thái hiện tại khác trạng thái trước
    button_msg_t button_msg;

    if(btn->button_curr_state != btn->button_last_state) {
        // trạng thái giống nhau là có nhấn nút
        if(btn->button_curr_state == btn->button_active_state) {
            btn->button_time_start_press = millis();
            btn->button_press_timeout_flag    = IS_PRESS_FLAG;
            // callback press function
            // Thường mình sẽ không xử lý tại đây vì nó có thể xung đột với các Event còn lại
            // Nên mình sẽ xử lý nhấn nhanh, nhấn nhả, nhấn lâu
            // Serial.println("NORMAL");
        }

        // thả nút
        else if(btn->button_curr_state != btn->button_active_state && btn->button_press_timeout_flag == IS_PRESS_FLAG) {
            // btn->button_press_timeout_flag == IS_PRESS_FLAG để khi mình nhấn lâu (LONG PRESS) mà có thả ra thì nó vẫn không vào hàm này

            button_msg.button_id = btn->button_pin;

            if(millis() - btn->button_time_start_press <= TIME_SHORT_PRESS) {
                // callback short press function
                button_msg.button_event = BUTTON_EVENT_SHORT_PRESS;
                xQueueSend(buttonQueue, &button_msg, 0);
                // send queue freeRTOS msg

                // Serial.println("SHORT");
            }

            else {
                // nếu chạy vào short press rồi thì không vào đây nữa
                button_msg.button_event = BUTTON_EVENT_RELEASE;
                xQueueSend(buttonQueue, &button_msg, 0);
                // send queue freeRTOS msg

                // Serial.println("RELEASE");
                // callback release function
            }

            btn->button_press_timeout_flag = IS_NON_PRESS_FLAG; // sau khi thả nút thì reset cái timeout flag không thôi nó dô hàm long press
        }
        btn->button_last_state = btn->button_curr_state;
    }

    // nhấn lâu -> khi thả ra sẽ chạy vào
    if(btn->button_press_timeout_flag && (millis() - btn->button_time_start_press >= TIME_LONG_PRESS)) {
        // callback long press function
        button_msg.button_event = BUTTON_EVENT_LONG_PRESS;
        xQueueSend(buttonQueue, &button_msg, 0);
        // send queue freeRTOS msg

        // Serial.println("LONG");
        btn->button_press_timeout_flag = IS_NON_PRESS_FLAG;
    }
}

static bool button_read_state(uint8_t pin) {
    return digitalRead(pin);
}

static bool button_config(button_debounce_t **btn, button_active_t act_sta) {
  /*
    int a = 10;
    int *p = &a;    // p trỏ đến a
    int **pp = &p;  // pp trỏ đến p
    Giải thích:

    pp chứa địa chỉ của p.

    *pp chính là p → địa chỉ của a.

    **pp chính là giá trị tại a → 10.
*/

    if(act_sta == HIGH_TO_LOW) {
        (*btn)->button_curr_state      = HIGH_STATE;
        (*btn)->button_filter_state    = HIGH_STATE;
        (*btn)->button_last_state      = HIGH_STATE;

        (*btn)->button_is_press            = IS_NON_PRESS_FLAG;
        (*btn)->button_press_timeout_flag  = IS_NON_PRESS_FLAG;

        // press time
        (*btn)->button_time_press          = 0;
        (*btn)->button_time_start_press    = 0;
        (*btn)->button_time_end_press      = 0;

        return 0;
    }

    (*btn)->button_curr_state      = LOW_STATE;
    (*btn)->button_filter_state    = LOW_STATE;
    (*btn)->button_last_state      = LOW_STATE;

    (*btn)->button_is_press            = IS_NON_PRESS_FLAG;
    (*btn)->button_press_timeout_flag  = IS_NON_PRESS_FLAG;

    // press time
    (*btn)->button_time_press          = 0;
    (*btn)->button_time_start_press    = 0;
    (*btn)->button_time_end_press      = 0;
    return 0;
}