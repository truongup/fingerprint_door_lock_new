#include "rfid_rc522.h"

int rc522_init(void) {
    EEPROM.begin(512); // Khởi tạo EEPROM
    SPI.begin();
    rfid.PCD_Init();

    // Đọc VersionReg để kiểm tra chip có phản hồi không
    byte ver = rfid.PCD_ReadRegister(rfid.VersionReg);

    if (ver == 0x91 || ver == 0x92) {
        log_d("MFRC522 connected. Version: 0x%x \n", ver);
        
        // set default key for reader buffer
        rc522_set_default_key();

        return FUNCTION_OK; // OK
    } 
    
    else {
        log_d("Không tìm thấy MFRC522. Kiểm tra dây nối!");
        return FUNCTION_FAIL; // Fail
    }

    return FUNCTION_OK; // OK
}

int rc522_read_card(void) {
    // Kiểm tra xem có thẻ (card) mới gì không, Đọc thẻ
    log_d("MFRC522 reading ...");
    if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial() ) {
        return FUNCTION_FAIL;
    }
    
    //  save buffer rfid
    rc522_get_nuid();

    return FUNCTION_OK;   // đọc thành công
}

void rc522_set_default_key(void) {
    for (byte idx = 0; idx < 6; idx++) {
        key.keyByte[idx] = 0xFF;
    }
}

void rc522_get_nuid(void) {
    // array that will store new NUID 
    for (byte idx = 0; idx < RFID_SIZE_CARD; idx++) {
        readNUID[idx] = rfid.uid.uidByte[idx];
    }
}

int rc522_compare(char *buffer) {
    for (int i = RFID_START_AT_EEPROM; i < NUMBER_OF_CARD_RFID; i++) {

        bool isOK = true;   // Biến xác định thẻ từ đúng

        for (int j = 0; j < RFID_SIZE_CARD; j++) {
            if (EEPROM.read(i * RFID_SIZE_CARD + j) != readNUID[j]) {
                isOK = false;
                break;
            }

            *buffer += *(readNUID + (RFID_SIZE_CARD - j - 1));
        }

        if(isOK) {
            return FUNCTION_OK;
        }
    }

    return FUNCTION_FAIL;
}