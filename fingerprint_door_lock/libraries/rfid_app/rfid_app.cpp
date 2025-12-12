#include "rfid_app.h"

int rfid_app_init(void) {
    return rc522_init();
}

int rfid_identify(char *rfid_id) {
    int ret = rc522_read_card();   // -1 is fail
    if(ret == FUNCTION_FAIL) {
        log_d("MFRC522 no card scanned ...");
        // no card scanned
        return FUNCTION_FAIL;
    } 

    // compare buffer 
    char buffer[10] = "";
    
    if(rc522_compare(buffer) == FUNCTION_FAIL) {
        // no valid rfid card
        log_d("MFRC522 card isn't valid ...");
        return FUNCTION_FAIL;
    }

    log_d("BUFFER_RFID: %s", buffer);
    strcpy(rfid_id, buffer);
    return FUNCTION_OK;

    // return tại đây luôn vì các tác vụ khác sẽ đưa vào queue handle

    // get buffer nuid
    // rc522_get_nuid();
    // xác thực ...
}