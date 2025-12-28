#ifndef TIME_APP_H_
#define TIME_APP_H_

#include "time.h"
#include "memory_struct.h"
static const char* ntpServer = "pool.ntp.org";

static int timezone = 7 * 3600;
static int dst = 0;

int time_app_init(void);
unsigned long time_app_get(void);

#endif