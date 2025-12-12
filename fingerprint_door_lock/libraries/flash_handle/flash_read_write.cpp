#include "flash_read_write.h"

int flash_read_write_init(void) {
    log_d("SPIFFS begin");
    if (!SPIFFS.begin()) {
        log_d("An Error has occurred while mounting SPIFFS");
        return -1;
    }
    return 0;
}

void flash_read_str_from_spiffs(const char *_filename, String& input_message) {
    //debug
    File f = SPIFFS.open(_filename, "r");

    if (!f) {
        Serial.println("file open failed");
    }

    else {
        //Serial.println("Reading Data from File:" + String(f.size()));
        //Data from file
        for (int i = 0; i < f.size(); i++) //Read upto complete file size
        {
            // *(input_message+i) = ((char)f.read());
            input_message += ((char)f.read());
        }
        // Serial.println(input_message);
        f.close(); //Close file
    }
    //debug
}

void flash_write_str_to_spiffs(const char *_filename, String &input_message) {
    // log_d("hello exit");

    if (!SPIFFS.exists(_filename)) {
        // log_d("hello exit");
        File f = SPIFFS.open(_filename, "w");
        // log_d("hello exit");
        f.print(input_message);
        // log_d("hello exit");
        f.close();
    }

    else {
        // log_d("hello exit");
        SPIFFS.remove(_filename);
        // log_d("hello exit");
        File f = SPIFFS.open(_filename, "w");
        // log_d("hello exit");
        f.print(input_message);
        // log_d("hello exit");
        f.close();
    }
    // log_d("hello exit");
}