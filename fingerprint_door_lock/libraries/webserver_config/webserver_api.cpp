#include "webserver_api.h"

void api_get_request_device_info(AsyncWebServerRequest *request) {
    DynamicJsonDocument doc(2048);

    JsonObject wifi_info = doc.createNestedObject("wifi_info");
    wifi_info["ssid"] = info_user.wifi_station.name;
    wifi_info["pass"] = info_user.wifi_station.pw;
    
    JsonObject firebase_info = doc.createNestedObject("firebase_info");
    firebase_info["api_key"]      = info_user.firebase_user.api_key;
    firebase_info["database_url"] = info_user.firebase_user.database_url;
    firebase_info["auth_email"]   = info_user.firebase_user.auth_email;
    firebase_info["auth_pw"]      = info_user.firebase_user.auth_pw;

    // convert json to string
    String output;
    serializeJson(wifi_info, output);

    request->send(200, "text/json", output);

    /*
        {
            "wifi_info": {
                "ssid": "tên wifi"
                "pass": "mật khẩu wifi"
            },
            
            "firebase_info": {
            
            }
        }
    */
}

void api_post_request_wifi_config(AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
    /**
     * data: con trỏ đến dữ liệu POST nhận được (kiểu mảng byte).
     * len: độ dài block dữ liệu nhận được lần này
     */
    char *temp = (char*)malloc((len+1)*sizeof(char));
    if(temp && (len <= 1024)) {
        temp[len] = '\0';                       // Đảm bảo buffer có ký tự kết thúc \0.
        memcpy((void*)temp, (void*)data, len);

        StaticJsonDocument<512> doc;
        DeserializationError error = deserializeJson(doc, (const char*)temp);   // convert json to doc json

        free(temp);

        if (error) {
            // sendFailRequest(request);
            request->send(200);
            return;
        }

        const char* ssid = doc["wifi_info"]["ssid"];   // đúng
        const char* pass = doc["wifi_info"]["pass"];     // đúng
            
        log_d("Update - WiFi Station Info: \n");
        log_d("ssid: %s \n", ssid);
        log_d("pass: %s \n", pass);

        char _ssid[128];
        char _pass[128];
        
        strcpy(_ssid, ssid);
        strcpy(_pass, pass);
        


        int result = change_info_wifi_station(_ssid, _pass);

        if(result == 0)
        {
            request->send(200, "application/json", "Change password complete!");
        }
        else
        {
            request->send(200, "application/json", "Change password fail!");
        }
        return;

        

    }

    // else
    request->send(200);
    return;
}