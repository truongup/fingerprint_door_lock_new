#include "firebase_app.h"
#include "addons/TokenHelper.h" //Provide the token generation process info.
#include "addons/RTDBHelper.h"  //Provide the RTDB payload printing info and other helper functions.

int firebase_app_init(void) {   
    firebase_cfg.api_key = info_user.firebase_user.api_key;               /* Assign the api key (required) */
    firebase_cfg.database_url = info_user.firebase_user.database_url;     /* Assign the RTDB URL (required) */

    // Assign the user sign in credentials
    firebase_auth.user.email = info_user.firebase_user.auth_email;
    firebase_auth.user.password = info_user.firebase_user.auth_pw;

    // Reconnect Firebase when lost connection
    Firebase.reconnectWiFi(true);
    firebase_data.setResponseSize(4096);
        
    // Assign the callback function for the long running token generation task
    firebase_cfg.token_status_callback = tokenStatusCallback;   //see addons/TokenHelper.h
    firebase_cfg.max_token_generation_retry = 5;                // Assign the maximum retry of token generation
    Firebase.begin(&firebase_cfg, &firebase_auth);              // Khởi tạo firebase

    // Lấy thông tin UserId - Sẽ mất 1 vài giây
    while ((firebase_auth.token.uid) == "") {
        delay(500);
    }

    // Get UID
    strcpy(info_user.firebase_user.uid, firebase_auth.token.uid.c_str());

    return FUNCTION_OK;
}

int firebase_app_load_path(void) {
    // chuẩn bị sẵn các path giao tiếp với firebase
    char temp[256];   
    memset(temp, 0, 256);

    // add new user (trigger ~ signal):     /store/uid/add_new_user
    strcat(temp, PATH_STORE);
    strcat(temp, info_user.firebase_user.uid);
    strcat(temp, PATH_ADD_NEW_USER);

    strcpy(firebase_path.add_new_user, temp);
    memset(temp, 0, 256);
    
    log_d("Path add new user: %s", firebase_path.add_new_user);

    // open door (trigger ~ signal):        /store/uid/open_door
    strcat(temp, PATH_STORE);
    strcat(temp, info_user.firebase_user.uid);
    strcat(temp, PATH_OPEN_DOOR);

    strcpy(firebase_path.open_door, temp);
    memset(temp, 0, 256);
    
    log_d("Path admin open door: %s", firebase_path.open_door);

    // upload finger:                       /store/uid/upload/finger_id
    strcat(temp, PATH_STORE);
    strcat(temp, info_user.firebase_user.uid);
    strcat(temp, PATH_UPLOAD);
    strcat(temp, PATH_FINGER_ID);

    strcpy(firebase_path.upload_finger_id, temp);
    memset(temp, 0, 256);
    
    log_d("Path upload template finger: %s", firebase_path.upload_finger_id);

    // upload rfid:                         /store/uid/upload/rfid_id
    strcat(temp, PATH_STORE);
    strcat(temp, info_user.firebase_user.uid);
    strcat(temp, PATH_UPLOAD);
    strcat(temp, PATH_RFID_ID);

    strcpy(firebase_path.upload_rfid_id, temp);
    memset(temp, 0, 256);
    
    log_d("Path upload template rfid: %s", firebase_path.upload_rfid_id);

    // send ID user open door by finger:    /store/uid/user_opened/finger_id
    strcat(temp, PATH_STORE);
    strcat(temp, info_user.firebase_user.uid);
    strcat(temp, info_user.firebase_user.user_opened);
    strcat(temp, PATH_USER_OPENED);
    strcat(temp, PATH_FINGER_ID);

    strcpy(firebase_path.usr_opened_by_finger, temp);
    memset(temp, 0, 256);

    log_d("Path upload user open door by finger: %s", firebase_path.usr_opened_by_finger);

    // send ID user open door by rfid:    /store/uid/user_opened/rifd_id
    strcat(temp, PATH_STORE);
    strcat(temp, info_user.firebase_user.uid);
    strcat(temp, info_user.firebase_user.user_opened);
    strcat(temp, PATH_USER_OPENED);
    strcat(temp, PATH_RFID_ID);

    strcpy(firebase_path.usr_opened_by_rfid, temp);
    memset(temp, 0, 256);

    log_d("Path upload user open door by rfid: %s", firebase_path.usr_opened_by_rfid);

    return FUNCTION_OK;
}

int firebase_trigger_add_new_user(void) {
    /**
        @path: /uid/store/add_new_user
        @param:
            uid: là id khi đăng nhập vào firebase

        @note: nếu không có tín hiệu thì nên xóa path để tránh xử lý lâu
    */
    if(Firebase.RTDB.getJSON(&firebase_data, firebase_path.add_new_user, &firebase_json)) {
        String json_string = firebase_data.to<FirebaseJson>().raw();

        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, json_string);

        if(error) {
            // log_d("ERR: %s", error.f_str());
            return FUNCTION_FAIL;
        }
        flag_block = FLAG_BLOCK_FIREBASE_TRIGGER;

        return FUNCTION_OK;
    }

    log_d("FIREBASE: no signal for add new user");
    return FUNCTION_FAIL;
}

int firebase_trigger_open_door(void) {
    if(Firebase.RTDB.getJSON(&firebase_data, firebase_path.open_door, &firebase_json)) {
        String json_string = firebase_data.to<FirebaseJson>().raw();

        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, json_string);

        if(error) {
            // log_d("ERR: %s", error.f_str());
            return FUNCTION_FAIL;
        }

        return FUNCTION_OK;
    }

    log_d("FIREBASE: no signal for open door");
    return FUNCTION_FAIL;
}

int firebase_send_template_finger_id(char *finger_id) {
    
}

int firebase_send_template_rfid_id(char *rfid_id) {

}

int firebase_send_user_opened(int open_type, uint8_t finger_id) {
    return FUNCTION_OK;
}

int firebase_send_user_opened(int open_type, String  rfid_id) {
    // path:    /store/uid/user_opened/finger_id

    // Firebase.RTDB.setInt

    return FUNCTION_OK;
}