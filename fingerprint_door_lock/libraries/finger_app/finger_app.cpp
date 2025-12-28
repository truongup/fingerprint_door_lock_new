#include "finger_app.h"

int finger_app_init(void) {
    return finger_as608_init();
}

int finger_app_enroll(uint8_t finger_enroll_id) {
    int ret = -1;
    
    // ===== set UI scan finger =====
    UIEvent event_ui = { 
        .type = UI_EVENT_FINGER_SCAN 
    };
    xQueueSend(uiEventQueue, &event_ui, 0);

    // ===== get finger at 1 times 1st =====
    oled_clear();
    oled_text("1st Scan", 1, 0, 0);
    vTaskDelay(10);
    while(finger_as608_get_image() != FINGERPRINT_OK) {
        vTaskDelay(20);
    }
    
    // ===== set UI valid finger =====
    event_ui = { 
        .type = UI_EVENT_FINGER_VALID,
        .clear_ui = CLEAR_UI
    };
    xQueueSend(uiEventQueue, &event_ui, 0);
    
    gpio_led_set(GPIO_DRIVE_LED_GREEN, LED_ON);
    vTaskDelay(1000);
    gpio_led_set(GPIO_DRIVE_LED_GREEN, LED_OFF);

    if(finger_as608_convert_image_to_template((uint8_t)1) != FINGERPRINT_OK);

    // ===== remove finger 2 seconds & waiting finger =====
    oled_clear();
    oled_text("Remove", 2, 30, 20);
    log_d("Remove Finger");
    vTaskDelay(pdMS_TO_TICKS(2000)); // chờ 2 giây để nhấc tay

    vTaskDelay(1000);

    // ===== set UI valid finger 2nd =====
    oled_clear();
    oled_text("2nd Scan", 1, 0, 0);

    event_ui = { 
        .type = UI_EVENT_FINGER_VALID 
    };
    xQueueSend(uiEventQueue, &event_ui, 0);

    // ===== get finger at 2 times =====
    log_d("ID %d", finger_enroll_id);
    
    while(finger_as608_get_image() != FINGERPRINT_OK) {
        vTaskDelay(20);
    }

    // ===== set UI valid finger =====
    event_ui = { 
        .type = UI_EVENT_FINGER_VALID,
        .clear_ui = CLEAR_UI
    };
    xQueueSend(uiEventQueue, &event_ui, 0);
    
    gpio_led_set(GPIO_DRIVE_LED_GREEN, LED_ON);
    vTaskDelay(1000);
    gpio_led_set(GPIO_DRIVE_LED_GREEN, LED_OFF);

    if(finger_as608_convert_image_to_template(2) != FINGERPRINT_OK);

    // ===== create model =====
    log_d("Creating model for #%d", finger_enroll_id);
    if(finger_as608_create_model() != FINGERPRINT_OK);

    // ===== store model =====
    log_d("Store model with ID: #%d", finger_enroll_id);
    if(finger_as608_store_model(finger_enroll_id) != FINGERPRINT_OK);

    vTaskDelay(1000);
    
    // UI RFID SCAN
    event_ui = { 
        .type = UI_EVENT_FINGER_VALID,
        .clear_ui = CLEAR_UI
    };
    xQueueSend(uiEventQueue, &event_ui, 0);
    return 0;
}

int finger_app_identify(uint8_t *finger_id) {
    int ret = -1;
    if(finger_as608_get_image() != FINGERPRINT_OK) return -1;

    if(finger_as608_convert_image_to_template(0) != FINGERPRINT_OK) return -1;
    
    uint8_t get_id = 0;
    
    // trong đây set UI luôn
    ret = finger_search(&get_id);
    
    *finger_id = get_id;
    return ret;
}

