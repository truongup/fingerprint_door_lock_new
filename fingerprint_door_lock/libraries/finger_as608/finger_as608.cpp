#include "finger_as608.h"

int finger_as608_init(void) {
    // redefine UART 
    fingerSerial.begin(FINGER_PRINT_BAUDRATE, SERIAL_8N1, FINGER_PRINT_RX_PIN, FINGER_PRINT_TX_PIN);
    // finger.begin(FINGER_PRINT_BAUDRATE);

    if (finger.verifyPassword()) {
        log_d("Found fingerprint sensor!");
        return 0;
    } 
    else {
        log_d("Did not find fingerprint sensor :(");
        return -1;
    }

    return 0;
}

void finger_as608_erase_memory(void) {
    finger.emptyDatabase();     // Xóa toàn bộ vân tay
}

int finger_as608_get_image(void) {
    int ret = -1;
    if (ret != FINGERPRINT_OK) {
        ret = finger.getImage();
        switch (ret) {
            case FINGERPRINT_OK:
                log_d("Image taken");
                break;

            case FINGERPRINT_NOFINGER:
                log_d(".");
                break;

            case FINGERPRINT_PACKETRECIEVEERR:
                log_d("Communication error");
                break;

            case FINGERPRINT_IMAGEFAIL:
                log_d("Imaging error");
                break;

            default:
                log_d("Unknown error");
                break;
        }
    }
    return ret;
}

int finger_as608_convert_image_to_template(uint8_t slot) {
    int ret;

    if(slot == 0)
        ret = finger.image2Tz();

    else 
        ret = finger.image2Tz(slot);
        
    switch (ret) {
        case FINGERPRINT_OK:
            log_d("Image converted");
            break;
            
        case FINGERPRINT_IMAGEMESS:
            log_d("Image too messy");
            break;

        case FINGERPRINT_PACKETRECIEVEERR:
            log_d("Communication error");
            break;

        case FINGERPRINT_FEATUREFAIL:
            log_d("Could not find fingerprint features");
            break;

        case FINGERPRINT_INVALIDIMAGE:
            log_d("Could not find fingerprint features");
            break;

        default:
            log_d("Unknown error");
            break;
    }
    return ret;
}

int finger_as608_create_model(void) {
    int ret = finger.createModel();
    if (ret == FINGERPRINT_OK) {
        log_d("Prints matched!");
    } 
    else if (ret == FINGERPRINT_PACKETRECIEVEERR) {
        log_d("Communication error");
    } 
    else if (ret == FINGERPRINT_ENROLLMISMATCH) {
        log_d("Fingerprints did not match");
    } 
    else {
        log_d("Unknown error");
    }
    return ret;
}

int finger_as608_store_model(uint8_t finger_enroll_id) {
    int ret = finger.storeModel(finger_enroll_id);
    if (ret == FINGERPRINT_OK) {
        log_d("Stored!");
    } 
    else if (ret == FINGERPRINT_PACKETRECIEVEERR) {
        log_d("Communication error");
    } 
    else if (ret == FINGERPRINT_BADLOCATION) {
        log_d("Could not store in that location");
    } 
    else if (ret == FINGERPRINT_FLASHERR) {
        log_d("Error writing to flash");
    } 
    else {
        log_d("Unknown error");
    }
    return ret;
}

int finger_search(uint8_t *finger_id) {
    int ret = finger.fingerSearch();
    if (ret == FINGERPRINT_OK) {
        UIEvent event_ui = { 
            .type = UI_EVENT_FINGER_VALID,
            .clear_ui = CLEAR_UI
        };
        xQueueSend(uiEventQueue, &event_ui, 0);

        flag_block = FLAG_BLOCK_IDENTIFY;

        *finger_id = finger.fingerID;
        log_d("Fingerprint matched");
    } 

    else if (ret == FINGERPRINT_PACKETRECIEVEERR) {
        log_d("Communication error");
    } 

    else if (ret == FINGERPRINT_NOTFOUND) {
        UIEvent event_ui = { 
            .type = UI_EVENT_FINGER_INVALID,
            .clear_ui =CLEAR_UI
        };
        xQueueSend(uiEventQueue, &event_ui, 0);

        flag_block = FLAG_BLOCK_IDENTIFY;

        log_d("Fingerprint not found");
    } 

    else {
        log_d("Unknown error");
    }

    return ret;
}