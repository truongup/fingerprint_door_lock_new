#include "cfg_hw.h"
#include "cfg_sw.h"
#include "wifi_app.h"
#include "memory_struct.h"
#include "memory_app.h"
#include "button_debounce.h"
#include "webserver_config.h"
#include "finger_app.h"
#include "oled_app.h"
#include "rfid_app.h"
#include "firebase_app.h"
#include "gpio_app.h"

/**
  Lưu ý: 
    Khi nạp phải nạp 2 file:
      - Code
      - Data

  ESP32 Version:  2.0.17

  Board:
    Tool => Board => ESP32 Dev Module 

  Nap data:
    arduino 1.18.xxx
  
  Build Code:
    
*/

TaskHandle_t my_task_handle_btn_app = NULL;
TaskHandle_t my_task_handle_identify = NULL;
TaskHandle_t my_task_trigger_from_firebase = NULL;
TaskHandle_t my_task_handle_ui = NULL;
// TaskHandle_t my_task_handle_ui = NULL;

// void vButtonAppTask(void *pvParameters) {
//   button_msg_t msg;
//   while(true) {
//     if(xQueueReceive(buttonQueue, &msg, portMAX_DELAY)) {
//       switch (msg.button_event) {
//         case BUTTON_EVENT_PRESS:
//           break;
      
//         case BUTTON_EVENT_RELEASE:
//           break;

//         case BUTTON_EVENT_SHORT_PRESS:
//           break;

//         case BUTTON_EVENT_LONG_PRESS:
//           //  Nếu là nút reset config -> chuyển sang chế độ cấu hình lại wifi 
//           if(msg.button_id == GPIO_BUTTON_RESET_CONFIG) {
//             log_d("RESET CONFIG DEVICE");
//           }
//           break;

//         default:
//           break;
//       }
//     }
//     vTaskDelay(20 / portTICK_PERIOD_MS);
//     yield();
//   }
// }

void vIdentifyTask(void *pvParameters) {
  while (true) {
    uint8_t finger_id = 0;
    String rfid_buffer = "";
    if(flag_block == FLAG_UN_BLOCK) {
      // Check xem có vân tay nào mở cửa không ?
      
      if(finger_app_identify(&finger_id) == FINGERPRINT_OK) {
        
        vTaskDelay(pdMS_TO_TICKS(100));
        
        firebase_send_user_opened(FINGER_OPENED, finger_id);
        
        vTaskDelay(pdMS_TO_TICKS(100));
        // enable gpio led, buzzer, solenoid
        gpio_buzzer_set(BUZZER_ON);
        gpio_led_set(GPIO_DRIVE_LED_GREEN, LED_ON);
        gpio_solenoid_set(SOLENOID_ON);
      }

      // Check xem có rfid nào mở cửa không ?
      
      else if(rfid_identify(&rfid_buffer) == FUNCTION_OK) {
        vTaskDelay(pdMS_TO_TICKS(100));
        gpio_buzzer_set(BUZZER_ON);
        gpio_led_set(GPIO_DRIVE_LED_RED, LED_OFF);
        gpio_led_set(GPIO_DRIVE_LED_GREEN, LED_ON);
        gpio_solenoid_set(SOLENOID_ON);

        vTaskDelay(pdMS_TO_TICKS(100));

        firebase_send_user_opened(RFID_OPENED, rfid_buffer);
        
      }
    }
    vTaskDelay(pdMS_TO_TICKS(100));
    yield();
  }
}

void vTriggerFromFirebaseTask(void *pvParameters) {
  while(true) {

    uint8_t usr_id_register = 0;

    if(flag_block == FLAG_UN_BLOCK) {
      if(firebase_trigger_add_new_user(&usr_id_register) == FUNCTION_OK) {
        log_d("FLAG BLOCK: %d", (uint8_t)flag_block);
        log_d("USR ID REGISTER: %d\n", usr_id_register);
        
        // đăng ký vân tay & gửi signal lên firebase (done)
        finger_app_enroll(usr_id_register);
        
        vTaskDelay(pdMS_TO_TICKS(100));

        firebase_send_template_finger_id(usr_id_register);  // usr_id_register == finger_id
        // while(1);

        // đăng ký rfid & gửi signal lên firebase
        String rfid_id = "";
        rfid_enroll(&rfid_id);

        vTaskDelay(pdMS_TO_TICKS(100));

        firebase_send_template_rfid_id(rfid_id);

        // taskbar chờ final
        // nếu mà vẫn còn path /add_new_user thì process đăng ký vẫn chưa kết thúc
        uint8_t led_status = LED_ON;
        while(firebase_trigger_add_new_user(&usr_id_register) == FUNCTION_OK) {
          gpio_led_set(GPIO_DRIVE_LED_GREEN, led_status);
          led_status = led_status == LED_ON ? LED_OFF : LED_ON;

          vTaskDelay(pdMS_TO_TICKS(150));
        }

        flag_block = FLAG_UN_BLOCK;

        UIEvent event_ui = { 
          .type = UI_EVENT_IDLE,
          .clear_ui = CLEAR_UI
        };
        xQueueSend(uiEventQueue, &event_ui, 0);
      }

      else if(firebase_trigger_open_door() == FUNCTION_OK) {
        firebase_response_admin_open_door();
        vTaskDelay(pdMS_TO_TICKS(50));
        firebase_response_admin_time_open_door();
        //  admin open door
        gpio_buzzer_set(BUZZER_ON);
        gpio_led_set(GPIO_DRIVE_LED_RED, LED_OFF);
        gpio_led_set(GPIO_DRIVE_LED_GREEN, LED_ON);
        gpio_solenoid_set(SOLENOID_ON);
      }
    }
    vTaskDelay(pdMS_TO_TICKS(100));
    yield();
  }
}

void vUIask(void *pvParameters) {
  UIEvent event_ui;
  while(true) {
    if (xQueueReceive(uiEventQueue, &event_ui, portMAX_DELAY) == pdTRUE) {     
      ui_timeout = millis();
      switch (event_ui.type) {
        // IDLE
        case UI_EVENT_IDLE:
          oled_idle(event_ui.clear_ui);
          break;

        // FINGER
        case UI_EVENT_FINGER_START:
          oled_finger_start();
          break;

        case UI_EVENT_FINGER_VALID:
          oled_finger_valid(event_ui.clear_ui);
          break;

        case UI_EVENT_FINGER_INVALID:
          oled_finger_invalid(event_ui.clear_ui);
          // gpio_led_set(GPIO_DRIVE_LED_RED, LED_ON);

          // set timeout 
          break;

        case UI_EVENT_FINGER_FAIL:
          oled_finger_fail();
          break;

        case UI_EVENT_FINGER_SCAN:
          oled_finger_scan();
          break;

        // RFID
        case UI_EVENT_RFID_SCAN:
          oled_rfid_scan(event_ui.clear_ui);
          break;
        
        case UI_EVENT_RFID_VALID:
          oled_rfid_valid(event_ui.clear_ui);
          break;
        
        case UI_EVENT_RFID_INVALID:
          oled_rfid_invalid(event_ui.clear_ui);
          break;
        
        // ADMIN
        case UI_EVENT_ADMIN_OPEN_DOOR:
          oled_admin_open_door(event_ui.clear_ui);
          break;

          // WAIT FINAL ADD USER
        case UI_EVENT_WAIT_FINAL_ADD_USER:
          oled_add_user_wait_process(event_ui.clear_ui);
          break;

        default:
            break;
      }
    }


    // tắt led + buzzer khi mở cửa thành công - thất bại
    while(flag_block == FLAG_BLOCK_IDENTIFY) {
      vTaskDelay(20 / portTICK_PERIOD_MS);
      if((millis() - ui_timeout) > UI_TIMEOUT) {
        oled_idle(CLEAR_UI);
        flag_block = FLAG_UN_BLOCK;

        switch(event_ui.type) {
          case UI_EVENT_FINGER_VALID:
          case UI_EVENT_RFID_VALID:
          case UI_EVENT_ADMIN_OPEN_DOOR:
            gpio_led_set(GPIO_DRIVE_LED_GREEN, LED_OFF);
            gpio_buzzer_set(BUZZER_OFF);
            gpio_solenoid_set(SOLENOID_OFF);
            break;

          case UI_EVENT_FINGER_INVALID:
          case UI_EVENT_RFID_INVALID:
            gpio_led_set(GPIO_DRIVE_LED_RED, LED_OFF);
            break;

          default:
            break;
        }
      }
      
    }

    vTaskDelay(pdMS_TO_TICKS(100));
    yield();
  }
}

void setup() {
  Serial.begin(9600);      
  Serial.printf("Reset reason: %d\n", esp_reset_reason()); 
  int ret = -1;   // return value

  Serial.printf("Flash chip size: %u bytes\n", ESP.getFlashChipSize());
  Serial.printf("Flash chip speed: %u Hz\n", ESP.getFlashChipSpeed());
  Serial.printf("Flash chip mode : %u\n", ESP.getFlashChipMode());

  // ======= memory ======= //
  if(memory_init() == FUNCTION_FAIL) while(true);

  ret = memory_flash_to_ram();

  // ======= oled ======= //
  oled_app_init();
  oled_wifi_start(info_user.wifi_station.name);

  // ======= wifi ======= //
  wifi_app_init();
  vTaskDelay(pdMS_TO_TICKS(100));
  

  // nếu lần đầu sử dụng thiết bị hoặc thiết bị đã bị reset thì khi load lên sẽ bật station mode để cài đặt
  if(operation_mode == CONFIG_DEVICE) {
    wifi_broadcast_for_setup();   // init wifi access point mode
    webserver_config_init();      // init web server local with ip from wifi access point mode
    while(true) {
      vTaskDelay(pdMS_TO_TICKS(50));
    }
  }

  oled_wifi_connected();

  vTaskDelay(pdMS_TO_TICKS(50));

  // ======= firebase ======= //
  if(firebase_app_init() == FUNCTION_FAIL) {
    while(true) {
      vTaskDelay(pdMS_TO_TICKS(50));
    }
  }

  vTaskDelay(pdMS_TO_TICKS(100));
  firebase_app_load_path();   // prepare path
  vTaskDelay(pdMS_TO_TICKS(100));

  // ======= finger ======= //
  if(finger_app_init() == FUNCTION_FAIL) {
    while(true) {
      vTaskDelay(pdMS_TO_TICKS(50));
    }
  }

  // ======= rfid ======= //
  if(rfid_app_init() == FUNCTION_FAIL) {
    while(true) {
      vTaskDelay(pdMS_TO_TICKS(50));
    }
  }

  // ======= gpio ======= //
  gpio_app_init();

  // // ======= button  & task button (Queue Task) ======= //
  button_init(&button_rst);
  // // xTaskCreate(vButtonAppTask, "Button App Task", 4096, NULL, 1, &my_task_handle_btn_app);
  
  // ======= task identify (Semaphore Mutex Task) ======= //
  xTaskCreate(vIdentifyTask, "Identify Task", 32768, NULL, 10, &my_task_handle_identify);
  
  // ======= task read trigger from firebase  ======= //
  xTaskCreate(vTriggerFromFirebaseTask, "Trigger From Firebase Task", 8192, NULL, 5, &my_task_trigger_from_firebase);

  // ======= task handle UI  ======= //
  uiEventQueue = xQueueCreate(10, sizeof(UIEvent));
  xTaskCreate(vUIask, "UI Task", 4096, NULL, 2, &my_task_handle_ui);

  vTaskDelay(1000);
  oled_idle(CLEAR_UI);
}

void loop() {
  button_handle(&button_rst);
   delay(100);
  yield();
}
