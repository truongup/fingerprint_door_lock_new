#ifndef  FLASH_READ_AND_WRITE_H
#define  FLASH_READ_AND_WRITE_H

#include "SPIFFS.h"

int flash_read_write_init(void);

void flash_read_str_from_spiffs(const char *_filename, String& input_message);

void flash_write_str_to_spiffs(const char *_filename, String &input_message);

#endif