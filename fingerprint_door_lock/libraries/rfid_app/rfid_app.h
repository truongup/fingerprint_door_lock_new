#ifndef RFID_APP_H
#define RFID_APP_H

#include "rfid_rc522.h"
#include "memory_struct.h"

int rfid_app_init(void);
int rfid_identify(String *rfid_id) ;
int rfid_enroll(String *rfid_id);
#endif