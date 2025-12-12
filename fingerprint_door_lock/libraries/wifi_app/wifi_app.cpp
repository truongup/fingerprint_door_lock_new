#include "wifi_app.h"

void wifi_app_init(void) {
    log_d("Wifi Setting Station Mode");
    log_d("Wifi Name: %s\n", info_user.wifi_station.name);
    log_d("Wifi Password: %s\n", info_user.wifi_station.pw);
    
    if(!strcmp(info_user.wifi_station.name, "cfg") && !strcmp(info_user.wifi_station.pw, "cfg")) {
        operation_mode = CONFIG_DEVICE;
        return;
    }

    // WiFi OK
    WiFi.begin(info_user.wifi_station.name, info_user.wifi_station.pw);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        log_d(".");
    }

    log_d("\n");
    log_d("WiFi connected.");
    log_d("IP address: %s", WiFi.localIP().toString());
}

void wifi_broadcast_for_setup(void) {
    log_d("Wifi Setting Access Point Mode");
    log_d("Wifi Name: %s\n", info_user.wifi_access_point.name);
    log_d("Wifi Password: %s\n", info_user.wifi_access_point.pw);       // password phải có ít nhất 8 kí tự

    WiFi.disconnect(true); 
    WiFi.mode(WIFI_AP);  

    bool wifi_init_ap_sta =  WiFi.softAP(
                                            info_user.wifi_access_point.name, 
                                            info_user.wifi_access_point.pw
                                        );

    if(wifi_init_ap_sta) {
        log_d("WiFi Access Point Mode Init Success\n");
    }

    else {
        log_d("WiFi Access Point Mode Init Failure\n");
    }

    IPAddress IP = WiFi.softAPIP();

    log_d("AP IP address: %s", IP.toString());
}   