#ifndef __BT_OTA_H__
#define __BT_OTA_H__

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/ringbuf.h"
#include "esp_err.h"

#include "sysdefs.h"

typedef void (*esp_ble_ota_recv_fw_cb_t)(uint8_t *buf, uint32_t length);

typedef struct esp_ble_ota_callback_funs {
    esp_ble_ota_recv_fw_cb_t recv_fw_cb;    /*!< BLE OTA data receive callback function */
} esp_ble_ota_callback_funs_t;

typedef struct esp_ble_ota_notification_check {
    bool recv_fw_ntf_enable;        /*!< BLE OTA receive firmware characteristic */
    bool command_ntf_enable;        /*!< BLE OTA command characteristic */
} esp_ble_ota_notification_check_t;

typedef enum {
    RECV_FW_CHAR,
    CMD_CHAR,
    INVALID_CHAR,
} esp_ble_ota_char_t;

typedef enum {
    RECV_FW_CHAR_VAL_IDX,
    CMD_CHAR_VAL_IDX,
} esp_ble_ota_service_index_t;

void bt_init(void);

#endif /* __BT_OTA_H__ */
