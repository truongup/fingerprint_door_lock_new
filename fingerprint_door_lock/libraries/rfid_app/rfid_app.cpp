#include "rfid_app.h"

int rfid_app_init(void) {
    return rc522_init();
}

int rfid_identify(String *rfid_id) {
    int ret = rc522_read_card();   // -1 is fail
    if(ret == FUNCTION_FAIL) {
        log_d("MFRC522 no card scanned ...");
        // no card scanned
        return FUNCTION_FAIL;
    } 

    // compare buffer 
    String buffer = "";
    
    if(rc522_compare(&buffer) == FUNCTION_FAIL) {
        // no valid rfid card
        log_d("MFRC522 card isn't valid ...");

        // set UI & block
        UIEvent event_ui = { 
            .type = UI_EVENT_RFID_INVALID,
            .clear_ui =CLEAR_UI
        };
        xQueueSend(uiEventQueue, &event_ui, 0);

        flag_block = FLAG_BLOCK_IDENTIFY;
        
        return FUNCTION_FAIL;
    }

    *rfid_id = buffer;

    // set UI & block
    UIEvent event_ui = { 
        .type = UI_EVENT_RFID_VALID,
        .clear_ui =CLEAR_UI
    };
    xQueueSend(uiEventQueue, &event_ui, 0);

    flag_block = FLAG_BLOCK_IDENTIFY;

    return FUNCTION_OK;

    // return tại đây luôn vì các tác vụ khác sẽ đưa vào queue handle

    // get buffer nuid
    // rc522_get_nuid();
    // xác thực ...
}

int rfid_enroll(String *rfid_id) {
    int index_memory = 0;
    int ret = rc522_eeprom_empty(&index_memory);
    if(ret == EEPROM_NO_EMPTY) {
        log_d("EEPROM is full\n");   
        return FUNCTION_FAIL;    // hết bộ nhớ
    }
    // Nếu bộ nhớ trống
    String buffer = "";
    
    // read card
    while(rc522_read_card() == FUNCTION_FAIL) {
        vTaskDelay(pdMS_TO_TICKS(100));
    }

    // thêm tính năng compare để xem card đã tồn tại chưa
    // ...

    rc522_eeprom_save(&buffer, index_memory);

    log_d("BUFFER_RFID: %s", buffer);
     *rfid_id = buffer;

    UIEvent event_ui = { 
        .type = UI_EVENT_WAIT_FINAL_ADD_USER,
        .clear_ui = CLEAR_UI
    };
    xQueueSend(uiEventQueue, &event_ui, 0);

    return FUNCTION_OK;
}