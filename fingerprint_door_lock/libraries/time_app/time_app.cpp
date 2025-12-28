#include "time_app.h"

int time_app_init(void) {
    configTime(timezone , dst, ntpServer);
    return FUNCTION_OK;
}

unsigned long time_app_get(void) {
    time_t now;
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      //Serial.println("Failed to obtain time");
      return(0);
    }
    time(&now);
    return now;
}