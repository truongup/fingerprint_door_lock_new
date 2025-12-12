#ifndef MEMORY_STRUCT_H
#define MEMORY_STRUCT_H

#include <Arduino.h>
#include <stdint.h>
#include "button_debounce.h"
#include "cfg_hw.h"
#include "cfg_sw.h"

// Operation Mode
typedef enum {
    ILDE,
    CONFIG_DEVICE,          // trạng thái chuyển sang cấu hình thiết bị
    
} operation_mode_t;

/*  =========================== INFO USER ===========================    */
// Contain Info Firebase
typedef struct {
    char api_key[256];
    char database_url[256];
    char auth_email[256];
    char auth_pw[256];
    char uid[256];

    char user_opened[256];
} firebase_user_t;

// Contain Info Wifi Station
typedef struct {
    char name[256];    // 256 byte
    char pw[256];      // 256 byte
} wifi_station_user_t;

// Contain Info Wifi Access Point
typedef struct {
    char name[256];   // 256 byte
    char pw[256];     // 256 byte
    uint8_t web_port;                   // 1 byte   - not use
} wifi_access_point_user_t;

typedef struct {
    wifi_station_user_t wifi_station;
    wifi_access_point_user_t wifi_access_point;
    firebase_user_t firebase_user;
} interface_data_user_t;


extern interface_data_user_t info_user;
extern operation_mode_t      operation_mode;

/*  =========================== FIREBASE PATH ===========================    */
typedef struct {
    char add_new_user[128];     
    char open_door[128];      
    
    // add new user template
    char upload_finger_id[128]; // uid/store/upload/finger_id/
    char upload_rfid_id[128];

    // user opened door
    char usr_opened_by_finger[128];
    char usr_opened_by_rfid[128];
    
} firebase_path_t;

extern firebase_path_t firebase_path;

/*  =========================== BUTTON ===========================    */
extern button_debounce_t button_rst;

/*  =========================== UI ===========================    */
typedef enum {
    UI_EVENT_NONE = 0,

    UI_EVENT_FINGER_START,
    UI_EVENT_FINGER_VALID,
    UI_EVENT_FINGER_INVALID,
    UI_EVENT_FINGER_FAIL,
    UI_EVENT_FINGER_SCAN,
    // thêm các event khác nếu cần
} UIEventType;

typedef struct {
    UIEventType type;
    // Có thể thêm dữ liệu khác nếu cần
} UIEvent;

extern QueueHandle_t uiEventQueue;
extern long long     ui_timeout;
extern uint8_t       ui_active_event;
#define UI_TIMEOUT   1500  // 1.5s

/*  =========================== FLAG BLOCK ===========================    */
// Các cờ chặn (block) tính năng
typedef enum {
  FLAG_UN_BLOCK,  
  FLAG_BLOCK_IDENTIFY,  
  FLAG_BLOCK_FIREBASE_TRIGGER, 
  FLAG_BLOCK_UI              
} flag_block_t;

extern flag_block_t flag_block;
#endif