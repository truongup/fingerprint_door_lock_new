#ifndef FINGER_AS608_CORE_H
#define FINGER_AS608_CORE_H

#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>

#include "cfg_hw.h"
#include "cfg_sw.h"
#include "memory_struct.h"

static HardwareSerial fingerSerial(FINGER_PRINT_HARDWARE_SERIAL); // Khởi tạo Serial1
static Adafruit_Fingerprint finger = Adafruit_Fingerprint(&fingerSerial);

int finger_as608_init(void);
void finger_as608_erase_memory(void);
int finger_as608_get_image(void);
int finger_search(uint8_t *finger_id);
int finger_as608_convert_image_to_template(uint8_t slot);
int finger_as608_create_model(void);
int finger_as608_store_model(uint8_t finger_enroll_id);
#endif