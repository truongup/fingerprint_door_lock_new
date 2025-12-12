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

void vButtonAppTask(void *pvParameters) {
  button_msg_t msg;
  while(true) {
    if(xQueueReceive(buttonQueue, &msg, portMAX_DELAY)) {
      switch (msg.button_event) {
        case BUTTON_EVENT_PRESS:
          break;
      
        case BUTTON_EVENT_RELEASE:
          break;

        case BUTTON_EVENT_SHORT_PRESS:
          break;

        case BUTTON_EVENT_LONG_PRESS:
          //  Nếu là nút reset config -> chuyển sang chế độ cấu hình lại wifi 
          if(msg.button_id == GPIO_BUTTON_RESET_CONFIG) {
            log_d("RESET CONFIG DEVICE");
          }
          break;

        default:
          break;
      }
    }
    vTaskDelay(20 / portTICK_PERIOD_MS);
    yield();
  }
}

void vIdentifyTask(void *pvParameters) {
  while (true) {
    char rfid_buffer[10] = "";
    uint8_t finger_id = 0;

    if(flag_block == FLAG_UN_BLOCK) {
      if(finger_app_identify(&finger_id) == FINGERPRINT_OK) {
        log_d("FINGER ID: %d \n", finger_app_identify);
        firebase_send_user_opened(FINGER_OPENED, finger_id);
      }

      else if(rfid_identify(rfid_buffer) == FUNCTION_OK) {
        gpio_led_set(GPIO_DRIVE_LED_RED, LED_OFF);
        gpio_led_set(GPIO_DRIVE_LED_GREEN, LED_ON);
        gpio_solenoid_set(SOLENOID_ON);
        firebase_send_user_opened(RFID_OPENED, rfid_buffer);
      }
    }
    vTaskDelay(20 / portTICK_PERIOD_MS);
    yield();
  }

}

void vTriggerFromFirebaseTask(void *pvParameters) {
  while(true) {
    log_d("Firebase task go");

    if(flag_block == FLAG_UN_BLOCK) {
      if(firebase_trigger_add_new_user() == FUNCTION_OK) {
        // send queue
      }

      if(firebase_trigger_open_door() == FUNCTION_OK) {
        //  admin open door
        gpio_led_set(GPIO_DRIVE_LED_RED, LED_OFF);
        gpio_led_set(GPIO_DRIVE_LED_GREEN, LED_ON);
        gpio_solenoid_set(SOLENOID_ON);

        // send queue
      }
    }
    log_d("Firebase task out");
    vTaskDelay(20 / portTICK_PERIOD_MS);
    yield();
  }
}

void vUIask(void *pvParameters) {
  UIEvent event_ui;
  while(true) {
    log_d("UI Task");
    if (xQueueReceive(uiEventQueue, &event_ui, portMAX_DELAY) == pdTRUE) {     
      ui_timeout = millis();
      switch (event_ui.type) {
        case UI_EVENT_FINGER_START:
          oled_finger_start();
          break;

        case UI_EVENT_FINGER_VALID:
          gpio_buzzer_set(BUZZER_ON);
          gpio_led_set(GPIO_DRIVE_LED_GREEN, LED_ON);
          gpio_solenoid_set(SOLENOID_ON);
          oled_finger_valid();

          // set timeout 
          ui_active_event = UI_EVENT_ACTIVE; 
          break;

        case UI_EVENT_FINGER_INVALID:
          oled_finger_invalid();
          gpio_led_set(GPIO_DRIVE_LED_RED, LED_ON);

          // set timeout 
          ui_active_event = UI_EVENT_ACTIVE; 
          break;

        case UI_EVENT_FINGER_FAIL:
          oled_finger_fail();
          break;

        case UI_EVENT_FINGER_SCAN:
          oled_finger_scan();
          break;

        default:
            break;
      }
    }

    while(ui_active_event == UI_EVENT_ACTIVE) {
      vTaskDelay(20 / portTICK_PERIOD_MS);
      if((millis() - ui_timeout) > UI_TIMEOUT) {
        oled_idle();
        ui_active_event = UI_EVENT_INACTIVE; 
        flag_block = FLAG_UN_BLOCK;

        switch(event_ui.type) {
          case UI_EVENT_FINGER_VALID:
            gpio_led_set(GPIO_DRIVE_LED_GREEN, LED_OFF);
            gpio_buzzer_set(BUZZER_OFF);
            gpio_solenoid_set(SOLENOID_OFF);
            break;

          case UI_EVENT_FINGER_INVALID:
            gpio_led_set(GPIO_DRIVE_LED_RED, LED_OFF);
            break;

          default:
            break;
        }
      }
    }

    vTaskDelay(20 / portTICK_PERIOD_MS);
    yield();
  }
}

void setup() {
  Serial.begin(9600);       
  int ret = -1;   // return value

  // ======= memory ======= //
  if(memory_init() == FUNCTION_FAIL) while(true);

  ret = memory_flash_to_ram();

  // ======= oled ======= //
  oled_app_init();
  oled_wifi_start(info_user.wifi_station.name);

  // ======= wifi ======= //
  wifi_app_init();

  // nếu lần đầu sử dụng thiết bị hoặc thiết bị đã bị reset thì khi load lên sẽ bật station mode để cài đặt
  if(operation_mode == CONFIG_DEVICE) {
    wifi_broadcast_for_setup();   // init wifi access point mode
    webserver_config_init();      // init web server local with ip from wifi access point mode
    while(true);
  }

  oled_wifi_connected();
  
  // ======= firebase ======= //
  if(firebase_app_init() == FUNCTION_FAIL) while (true);
  firebase_app_load_path();   // prepare path

  // ======= finger ======= //
  if(finger_app_init() == FUNCTION_FAIL) while(true);

  // ======= rfid ======= //
  if(rfid_app_init() == FUNCTION_FAIL) while(true);

  // ======= gpio ======= //
  gpio_app_init();

  // ======= button  & task button (Queue Task) ======= //
  button_init(&button_rst);
  xTaskCreate(vButtonAppTask, "Button App Task", 4096, NULL, 1, &my_task_handle_btn_app);
  
  // ======= task identify (Semaphore Mutex Task) ======= //
  xTaskCreate(vIdentifyTask, "Identify Task", 32768, NULL, 10, &my_task_handle_identify);
  
  // ======= task read trigger from firebase  ======= //
  xTaskCreate(vTriggerFromFirebaseTask, "Trigger From Firebase Task", 8192, NULL, 5, &my_task_trigger_from_firebase);

  // ======= task handle UI  ======= //
  uiEventQueue = xQueueCreate(10, sizeof(UIEvent));
  xTaskCreate(vUIask, "UI Task", 4096, NULL, 2, &my_task_handle_ui);
}

void loop() {
  button_handle(&button_rst);
  delay(50);
  yield();
}
