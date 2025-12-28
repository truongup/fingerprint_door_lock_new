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

int rc522_compare(String *buffer) {
    for (int i = RFID_START_AT_EEPROM; i < NUMBER_OF_CARD_RFID; i++) {
        bool isOK = true;   // Biến xác định thẻ từ đúng

        for (int j = 0; j < RFID_SIZE_CARD; j++) {
            if (EEPROM.read(i * RFID_SIZE_CARD + j) != readNUID[j]) {
                isOK = false;
                break;
            }

            *buffer += *(readNUID + (RFID_SIZE_CARD - j - 1));
        }

        if(isOK) return FUNCTION_OK;
    }

    return FUNCTION_FAIL;

}

int rc522_eeprom_empty(int *index_memory) {
    for (int i = RFID_START_AT_EEPROM; i < NUMBER_OF_CARD_RFID; i++) {
        // Kiểm tra xem bộ nhớ tại vị trí i có đang trống không
        bool empty = true;
        for (int j = 0; j < RFID_SIZE_CARD; j++) {
            if (EEPROM.read(i * RFID_SIZE_CARD + j) != 0xFF) {
                empty = false;
                break;
            }
        }
        if(empty) {
            *index_memory = i;      // lấy vị trí trống đầu tiên trong bộ nhớ eeprom
            return EEPROM_IS_EMPTY;
        } 
    }

    return EEPROM_NO_EMPTY;

    

    return EEPROM_NO_EMPTY;
}

int rc522_eeprom_save(String *buffer, int index_memory) {
    // log ra thử readNUID
    // Serial.print("RFID UID (HEX): ");
    // for (uint8_t i = 0; i < RFID_SIZE_CARD; i++) {
    //     if (readNUID[i] < 0x10) Serial.print("0");   // thêm 0 cho đẹp
    //     Serial.print(readNUID[i], HEX);
    //     if (i < RFID_SIZE_CARD - 1) Serial.print(":");
    // }
    // Serial.println();

    for (int j = 0; j < RFID_SIZE_CARD; j++) {
        // Lưu trong eeprom thì sẽ lưu theo chiều thuận của UID
        EEPROM.write(index_memory * RFID_SIZE_CARD + j, readNUID[j]);

        // lưu vào buffer (on firebase) thì sẽ lưu ngược lại
        *buffer += *(readNUID + (RFID_SIZE_CARD - j - 1));
    }
    // buffer[RFID_SIZE_CARD] = '\0';

    // commit lên bộ nhớ eeprom
    EEPROM.commit(); 

    log_d("readNUID: %s", readNUID);
    return FUNCTION_OK;
}