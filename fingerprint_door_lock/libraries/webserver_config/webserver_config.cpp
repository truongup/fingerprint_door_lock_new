#include "webserver_config.h"

AsyncWebServer *server;

void webserver_config_init(void) {
    log_d("%d", info_user.wifi_access_point.web_port);
    server = new AsyncWebServer(info_user.wifi_access_point.web_port);  // port 80

    webserver_source_init();
    webserver_api_init();

    server->begin();
}

static void webserver_source_init(void) {
    // resource web consist of html, css, js
        
    server->on(PSTR("/"), HTTP_GET, [](AsyncWebServerRequest *request) {
        // Route for root / web page
        request->send(SPIFFS, "/index.html", "text/html");          // lưu ý khi dùng SPIFFS
        // request->send(LittleFS, "/index.html", "text/html");     // lưu ý khi dùng LittleFS
    });

    server->on(PSTR("/resources/css/style.css"), HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/resources/css/style.css", "text/css");      // lưu ý khi dùng SPIFFS
        // request->send(LittleFS, "/resources/css/style.css", "text/css");    // lưu ý khi dùng LittleFS
    });

    server->on(PSTR("/resources/js/script.js"), HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/resources/js/script.js", "text/javascript");
    });
}

static void webserver_api_init(void) {
    // esp is server
    // send text: "text/plain"
    // send json: 
    server->on("/device_info", HTTP_GET, api_get_request_device_info);    // send to client 

    server->on("/wifi_update", HTTP_POST, [](AsyncWebServerRequest * request){}, NULL, api_post_request_wifi_config);

    server->on("/wifi_update", HTTP_POST, [](AsyncWebServerRequest * request){}, NULL, api_post_request_wifi_config);
}