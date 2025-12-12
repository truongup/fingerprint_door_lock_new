#ifndef  FLASH_READ_AND_WRITE_V2_H
#define  FLASH_READ_AND_WRITE_V2_H

#include "LittleFS.h"

int flash_read_write_init_v2(void);

void flash_read_str_from_littlefs(const char *_filename, String& input_message);

void flash_write_str_to_littlefs(const char *_filename, String &input_message);

#endif