#include "flash_read_write_v2.h"

int flash_read_write_init_v2(void) {
    // log_d("SPIFFS begin");
    if (!LittleFS.begin(true)) {
        // log_d("An Error has occurred while mounting SPIFFS");
        return -1;
    }
    return 0;
}

void flash_read_str_from_littlefs(const char *_filename, String& input_message) {
    //debug
    File file = LittleFS.open(_filename, "r");

    if (!file) {
        Serial.println("file open failed");
    }

    else {
        //Serial.println("Reading Data from File:" + String(f.size()));
        //Data from file
        for (int i = 0; i < file.size(); i++) //Read upto complete file size
        {
            // *(input_message+i) = ((char)f.read());
            input_message += ((char)file.read());
        }
        // Serial.println(input_message);
        file.close(); //Close file
    }
    //debug
}

void flash_write_str_to_spiffs(const char *_filename, String &input_message) {
    // log_d("hello exit");

    if (!LittleFS.exists(_filename)) {
        // log_d("hello exit");
        File file = LittleFS.open(_filename, "w");
        // log_d("hello exit");
        file.print(input_message);
        // log_d("hello exit");
        file.close();
    }

    else {
        // log_d("hello exit");
        LittleFS.remove(_filename);
        // log_d("hello exit");
        File file = LittleFS.open(_filename, "w");
        // log_d("hello exit");
        file.print(input_message);
        // log_d("hello exit");
        file.close();
    }
    // log_d("hello exit");
}