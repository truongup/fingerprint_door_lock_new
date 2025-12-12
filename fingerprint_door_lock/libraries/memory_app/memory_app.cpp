#include "memory_app.h"

int memory_init(void) {
    int ret = flash_read_write_init();             // init SPIFFS 
    
    if(ret == FUNCTION_FAIL) {
        log_d("Memory Flash Fail\n");
        return FUNCTION_FAIL;
    }

    return FUNCTION_OK;

    // return flash_read_write_init_v2();             //      LittleFS
}

int memory_flash_to_ram(void) {
    String data_read_from_json_file;            //  đọc giá trị từ file json
    DynamicJsonDocument data_json(2048);        //  chứa giá trị json sau khi convert từ string sang json
    DeserializationError error;                 //  nếu error khi giải mã từ string sang json thì error sẽ nhận flag
    char temp[256];                             //  dùng để chứa chuỗi tạm

    /*  =========================== WIFI STATION ===========================    */
    flash_read_str_from_spiffs(FILE_CONFIG_WIFI, data_read_from_json_file);             // SPIFFS
    // flash_read_str_from_littlefs(FILE_CONFIG_WIFI, data_read_from_json_file);        // LittleFS

    error = deserializeJson(data_json, data_read_from_json_file);   

    if(error) {
        log_d("%s\n", error.c_str());
        return -1;
    }
    
    JsonObject wifi_station_json = data_json["station"];
    const char* wifi_station_name   = wifi_station_json["wifi_name"];   // "admin"
    const char* wifi_station_pw     = wifi_station_json["wifi_pw"];     // "admin"

    // set wifi name
    memset(temp, 0, 256);
    strcpy(temp, wifi_station_name);
    strcpy(info_user.wifi_station.name, temp);

    // set password's wifi
    memset(temp, 0, 256);
    strcpy(temp, wifi_station_pw);
    strcpy(info_user.wifi_station.pw, temp);

    /*  =========================== WIFI ACCESS POINT ===========================    */
    JsonObject wifi_access_point_json = data_json["access_point"];
    const char* wifi_access_point_name   = wifi_access_point_json["wifi_name"];   // "admin"
    const char* wifi_access_point_pw     = wifi_access_point_json["wifi_pw"];     // "admin"

    // set wifi name
    memset(temp, 0, 256);
    strcpy(temp, wifi_access_point_name);
    strcpy(info_user.wifi_access_point.name, temp);

    // set password's wifi
    memset(temp, 0, 256);
    strcpy(temp, wifi_access_point_pw);
    strcpy(info_user.wifi_access_point.pw, temp);

    // web port: 80
    info_user.wifi_access_point.web_port = wifi_access_point_json["web_port"]; 

    /*  =========================== FIREBASE CONFIG ===========================    */
    data_read_from_json_file = "";
    flash_read_str_from_spiffs(FILE_FIREBASE_CONFIG, data_read_from_json_file);             // SPIFFS
    
    error = deserializeJson(data_json, data_read_from_json_file);   

    if(error) {
        log_d("%s\n", error.c_str());
        return -1;
    }
    
    const char* firebase_api_key        = data_json["firebase_api_key"];            
    const char* firebase_database_url   = data_json["firebase_realtime_database_url"];   
    const char* firebase_auth_email     = data_json["firebase_device_email"];   
    const char* firebase_auth_pw        = data_json["firebase_device_pw"];   

    // set firebase api key
    strcpy(info_user.firebase_user.api_key, firebase_api_key);

    // set firebase database url (realtime database)
    strcpy(info_user.firebase_user.database_url, firebase_database_url);

    // set firebase device auth email
    strcpy(info_user.firebase_user.auth_email, firebase_auth_email);

    // set firebase device auth pw
    strcpy(info_user.firebase_user.auth_pw, firebase_auth_pw);

    log_d("database url: %s\n", info_user.firebase_user.database_url);

    return 0;
}

int change_info_wifi_station(char *new_ssid, char *new_pass) {
    StaticJsonDocument<256> doc;

    JsonObject station = doc.createNestedObject("station");
    station["wifi_name"] = new_ssid;
    station["wifi_pw"]   = new_pass;

    // cái này update sau vì nếu bỏ sót vẽ mất luôn cái này
    JsonObject ap = doc.createNestedObject("access_point");
    ap["wifi_name"] = "FINGER_DOOR_LOCK";
    ap["wifi_pw"]   = "123456789";
    ap["web_port"]  = 80;

    // Xuất ra chuỗi JSON
    String output;
    serializeJson(doc, output);
    Serial.println(output);

    flash_write_str_to_spiffs(FILE_CONFIG_WIFI, output);
    return 0;
}