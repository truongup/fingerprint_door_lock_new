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
    
    // history time for admin
    char admin_time_open[128];
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

    UI_EVENT_RFID_SCAN,
    UI_EVENT_RFID_VALID,
    UI_EVENT_RFID_INVALID,

    UI_EVENT_ADMIN_OPEN_DOOR,

    UI_EVENT_WAIT_FINAL_ADD_USER,   // đợi web gửi tín hiệu kết thúc (process bar)

    UI_EVENT_IDLE,                  // giao diện khi bình thường
} UIEventType;

#define NON_CLEAR_UI 0
#define CLEAR_UI     1
typedef struct {
    UIEventType type;
    uint8_t     clear_ui;
    // Có thể thêm dữ liệu khác nếu cần
} UIEvent;

extern QueueHandle_t uiEventQueue;
extern long long     ui_timeout;
extern uint8_t       ui_active_event;
#define UI_TIMEOUT   2500  // 1.5s

/*  =========================== FLAG BLOCK ===========================    */
// Các cờ chặn (block) tính năng
typedef enum {
  FLAG_UN_BLOCK = 0,  
  FLAG_BLOCK_IDENTIFY = 1,  
  FLAG_BLOCK_FIREBASE_TRIGGER = 2, 
  FLAG_BLOCK_UI = 3              
} flag_block_t;

extern flag_block_t flag_block;
#endif