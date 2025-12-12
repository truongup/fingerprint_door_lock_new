#include "finger_app.h"

int finger_app_init(void) {
    return finger_as608_init();
}

int finger_app_enroll(uint8_t finger_enroll_id) {
    int ret = -1;

    // ===== get finger at 1 times =====
    vTaskDelay(10);
    if(finger_as608_get_image() != FINGERPRINT_OK) return -1;

    if(finger_as608_convert_image_to_template((uint8_t)1) != FINGERPRINT_OK) return -1;

    // ===== remove finger 2 seconds & waiting finger =====
    log_d("Remove Finger");
    vTaskDelay(pdMS_TO_TICKS(2000)); // chờ 2 giây để nhấc tay

    ret = -1; 
    while (ret == FINGERPRINT_OK) {
        ret = finger_as608_get_image();
        vTaskDelay(pdMS_TO_TICKS(50));
    }

    // ===== get finger at 2 times =====
    log_d("ID %d", finger_enroll_id);
    
    if(finger_as608_get_image() != FINGERPRINT_OK) return -1;

    if(finger_as608_convert_image_to_template(2) != FINGERPRINT_OK) return -1;

    // ===== create model =====
    log_d("Creating model for #%d", finger_enroll_id);
    if(finger_as608_create_model() != FINGERPRINT_OK) return -1;

    // ===== store model =====
    log_d("Store model with ID: #%d", finger_enroll_id);
    if(finger_as608_store_model(finger_enroll_id) != FINGERPRINT_OK)  return -1;

    return 0;
}

int finger_app_identify(uint8_t *finger_id) {
    int ret = -1;
    if(finger_as608_get_image() != FINGERPRINT_OK) return -1;

    if(finger_as608_convert_image_to_template(0) != FINGERPRINT_OK) return -1;
    
    uint8_t get_id = 0;
    ret = finger_search(&get_id);
    
    *finger_id = get_id;
    return ret;
}

