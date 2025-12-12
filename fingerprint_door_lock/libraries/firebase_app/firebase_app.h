#ifndef FIREBASE_APP_H
#define FIREBASE_APP_H

#include "cfg_sw.h"
#include "memory_struct.h"

#include <Firebase_ESP_Client.h>
#include <ArduinoJson.h>

#include "string.h"

// Firebase Component
static FirebaseData     firebase_data;       
static FirebaseAuth     firebase_auth;
static FirebaseConfig   firebase_cfg;
static FirebaseJson     firebase_json;   
// Example UID: 8pN513xWDacJtxHqzaIYJLpWrRp2

#define PATH_STORE          "/store/"
#define PATH_ADD_NEW_USER   "/add_new_user"
#define PATH_UPLOAD         "/upload/"
#define PATH_FINGER_ID      "/finger_id"
#define PATH_RFID_ID        "/rifd_id"

#define PATH_USER_OPENED    "/user_opened"

#define PATH_OPEN_DOOR      "/open_door"    

int firebase_app_init(void);
int firebase_app_load_path(void);
// 
int firebase_trigger_add_new_user(void);        // read from firebase
int firebase_trigger_open_door(void);           // read from firebase

int firebase_send_template_finger_id(char *finger_id);   // send to firebase
int firebase_send_template_rfid_id(char *rfid_id);       // send to firebase

int firebase_send_user_opened(int open_type, uint8_t finger_id);
int firebase_send_user_opened(int open_type, String  rfid_id);


#endif


