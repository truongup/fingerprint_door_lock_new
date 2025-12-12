#ifndef CONFIG_SOFTWARE_H
#define CONFIG_SOFTWARE_H

#define  FUNCTION_FAIL -1
#define  FUNCTION_OK    0

#define RESET           0
#define SET             1

#define SOLENOID_ON     SET
#define SOLENOID_OFF    RESET

#define LED_ON          SET
#define LED_OFF         RESET

#define BUZZER_ON       SET
#define BUZZER_OFF      RESET

#define FINGER_OPENED   0
#define RFID_OPENED     1

#define UI_EVENT_ACTIVE     SET
#define UI_EVENT_INACTIVE   RESET

#endif