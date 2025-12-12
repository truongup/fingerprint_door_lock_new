#ifndef RFID_APP_H
#define RFID_APP_H

#include "rfid_rc522.h"

int rfid_app_init(void);
int rfid_identify(char *rfid_id);

#endif