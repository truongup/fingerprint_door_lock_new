#ifndef RFID_RC522_CORE_H
#define RFID_RC522_CORE_H

#include <SPI.h>
#include <MFRC522.h>  // dùng lib gốc của miguelbalboa
#include "cfg_sw.h"
#include "cfg_hw.h"
#include <EEPROM.h>

#define NUMBER_OF_CARD_RFID     50
#define RFID_SIZE_CARD          4
#define RFID_START_AT_EEPROM    0
// #define RFID_END_AT_EEPROM      

#define EEPROM_NO_EMPTY 0
#define EEPROM_IS_EMPTY 1

static MFRC522 rfid(RFID_SPI_SS_PIN, RFID_SPI_RST_PIN);
static MFRC522::MIFARE_Key key; 
static byte readNUID[RFID_SIZE_CARD];   // Init array that will store new NUID 

int rc522_init(void);
void rc522_set_default_key(void);
int rc522_read_card(void);

void rc522_get_nuid(void);
int rc522_compare(String *buffer);

int rc522_eeprom_empty(int *index_memory);
int rc522_eeprom_save(String *buffer, int index_memory);
#endif
