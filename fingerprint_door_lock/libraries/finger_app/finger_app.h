#ifndef FINGER_APP_H
#define FINGER_APP_H

#include "finger_as608.h"
#include "oled_app.h"

int finger_app_init(void);
int finger_app_enroll(uint8_t finger_enroll_id);
int finger_app_identify(uint8_t *finger_id);          // kiểm tra xem vân tay có hợp lệ không, hợp lệ trả về ID của vân tay đó

#endif