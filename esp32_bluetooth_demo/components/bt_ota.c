#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/ringbuf.h"
#include "esp_ota_ops.h"
#include "nvs_flash.h"
#include "esp_bt.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h"
#include "host/util/util.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"
#include "host/ble_uuid.h"
#include "sdkconfig.h"

#include "sysdefs.h"
#include "main.h"
#include "../components/bt_ota.h"

#define OTA_RINGBUF_SIZE 8192
#define OTA_TASK_SIZE 8192
#define BUF_LENGTH 4098
#define OTA_IDX_NB 2
#define CMD_ACK_LENGTH 20
#define GATT_SVR_SVC_ALERT_UUID 0x1811
#define RECV_FW_UUID 0x8020
#define COMMAND_UUID 0x8022

static const char *TAG = "Ble";

static int esp_ble_ota_notification_data(uint16_t conn_handle, uint16_t attr_handle, uint8_t cmd_ack[], esp_ble_ota_char_t ota_char);
static int ble_ota_gatt_handler(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg);
static int deviceReadSwVer(uint16_t conHandle, uint16_t attrHandle, struct ble_gatt_access_ctxt *ctxt, void *arg);
static int deviceReadHwVer(uint16_t conHandle, uint16_t attrHandle, struct ble_gatt_access_ctxt *ctxt, void *arg);
static int deviceReadMfr(uint16_t conHandle, uint16_t attrHandle, struct ble_gatt_access_ctxt *ctxt, void *arg);
static void gatt_svr_register_cb(struct ble_gatt_register_ctxt *ctxt, void *arg);
esp_err_t esp_ble_ota_recv_fw_data_callback(esp_ble_ota_recv_fw_cb_t callback);
static esp_ble_ota_char_t find_ota_char_and_desr_by_handle(uint16_t handle);
static esp_err_t esp_ble_ota_recv_fw_handler(uint8_t *buf, uint32_t length);
static int esp_ble_ota_gap_event(struct ble_gap_event *event, void *arg);
static size_t write_to_ringbuf(const uint8_t *data, size_t size);
static uint16_t crc16_ccitt(const unsigned char *buf, int len);
static void ota_recv_fw_cb(uint8_t *buf, uint32_t length);
static bool ble_ota_ringbuf_init(uint32_t ringbuf_size);
static void esp_ble_ota_host_task(void *param);
static void esp_ble_ota_on_reset(int reason);
static int ble_ota_gatt_svr_init(void);
static void esp_ble_ota_on_sync(void);
esp_err_t esp_ble_ota_host_init(void);
static void resetCurrentPacket(void);
static void resetCurrentSector(void);
void ble_store_config_init(void);
static void ota_task(void *arg);

const char *deviceName;
const char *manufacturer;
const char *swVer;
const char *hwVer;
static bool start_ota = false;
static uint32_t cur_sector = 0;
static uint32_t cur_packet = 0;
static uint8_t *fw_buf = NULL;
static uint32_t fw_buf_offset = 0;
static uint32_t ota_total_len = 0;
static uint32_t ota_block_size = BUF_LENGTH;
static uint8_t own_addr_type;
static uint16_t connection_handle;
static uint16_t attribute_handle;
static uint16_t ota_handle_table[OTA_IDX_NB];
static uint16_t receive_fw_val;
static uint16_t command_val;
uint16_t notificationHandle;
uint16_t connHandle;

static RingbufHandle_t s_ringbuf = NULL;
esp_ble_ota_callback_funs_t ota_cb_fun_t = { .recv_fw_cb = NULL };
esp_ble_ota_notification_check_t ota_notification = {
  .recv_fw_ntf_enable = false,
  .command_ntf_enable = false,
};


/*######################################  DEMO SECTIOON  ######################################   */
enum {
  LEVEL_OFF = 0,
  LEVEL_1,
  LEVEL_2,

  MAX_NUM_OF_LEVEL
};
/* Set treatment time */
static void setTime(uint16_t time) {
  main_setTime(time);
}

/* Control LED output state */
static void setLevel(uint8_t state) {
  switch (state) {
    /* Output off */
    case LEVEL_OFF:
      main_setLevel(state);
      break;

      /* Output level 1 */
    case LEVEL_1:
      main_setLevel(state);
      break;

      /* Output level 2 */
    case LEVEL_2:
      main_setLevel(state);
      break;

      /* Fallthrough */
    case MAX_NUM_OF_LEVEL:
    default:
      break;
  }
}

/* Set treatment time and control LED output state */
static void startTreatment(uint16_t time, uint8_t state) {
  switch (state) {
    /* Output off */
    case LEVEL_OFF:
      main_setLevel(state);
      break;

      /* Output level 1 */
    case LEVEL_1:
      main_setLevel(state);
      break;

      /* Output level 2 */
    case LEVEL_2:
      main_setLevel(state);
      break;

      /* Fallthrough */
    case MAX_NUM_OF_LEVEL:
    default:
      break;
  }
  main_setTime(time);
}

static int debugLed1(uint16_t handle, uint16_t attr, struct ble_gatt_access_ctxt *ctxt, void *arg);
static int debugLed2(uint16_t handle, uint16_t attr, struct ble_gatt_access_ctxt *ctxt, void *arg);
static int debugLed3(uint16_t handle, uint16_t attr, struct ble_gatt_access_ctxt *ctxt, void *arg);

static int debugLed1(uint16_t handle, uint16_t attr, struct ble_gatt_access_ctxt *ctxt, void *arg) {
  ESP_LOGI(TAG, "UUID: 0xFFF0 received");
	uint8_t data = 0;

	if ((ctxt->om->om_data)) {
		data = *(uint8_t *)ctxt->om->om_data;
	}
  ESP_LOGI(TAG, "Data: %d.", data);
  main_setTime(data);

	return 0;
}
static int debugLed2(uint16_t handle, uint16_t attr, struct ble_gatt_access_ctxt *ctxt, void *arg) {
  ESP_LOGI(TAG, "UUID: 0xFFF1 received");
	uint8_t data = 0;

	if ((ctxt->om->om_data)) {
		data = *(uint8_t *)ctxt->om->om_data;
	}
  ESP_LOGI(TAG, "Data: %d.", data);
  main_setLevel(data);

	return 0;
}
static int debugLed3(uint16_t handle, uint16_t attr, struct ble_gatt_access_ctxt *ctxt, void *arg) {
  ESP_LOGI(TAG, "UUID: 0xFFF1 received");
	uint8_t data = 0;

	if ((ctxt->om->om_data)) {
		data = *(uint8_t *)ctxt->om->om_data;
	}
  ESP_LOGI(TAG, "Data: %d.", data);
  main_setLevel(data);
  main_setTime(10);

	return 0;
}
/*################################################################################################*/






static const struct ble_gatt_svc_def ota_gatt_db[] = {
  { /* Default information - remove this is you like */
    .type = BLE_GATT_SVC_TYPE_PRIMARY,
    .uuid = BLE_UUID16_DECLARE(0x180A),
    .characteristics = (struct ble_gatt_chr_def[]) {
      {  /* Manufacturer String */
        .uuid = BLE_UUID16_DECLARE(0x2A29),
        .flags = BLE_GATT_CHR_F_READ,
        .access_cb = deviceReadMfr },
      { /* Software revision String */
        .uuid = BLE_UUID16_DECLARE(0x2A28),
        .flags = BLE_GATT_CHR_F_READ,
        .access_cb = deviceReadSwVer },
      { /* Hardware revision String */
        .uuid = BLE_UUID16_DECLARE(0x2A27),
        .flags = BLE_GATT_CHR_F_READ,
        .access_cb = deviceReadHwVer },

      { 0 } } },

  /* Implement custom bluetooth characteristics here */
  { .type = BLE_GATT_SVC_TYPE_PRIMARY,
    .uuid = BLE_UUID16_DECLARE(0xFFFF),
    .characteristics =
      (struct ble_gatt_chr_def[]) {
        { .uuid = BLE_UUID16_DECLARE(0xFFF0),
          .flags = BLE_GATT_CHR_F_WRITE,
          .access_cb = debugLed1 },
        { .uuid = BLE_UUID16_DECLARE(0xFFF1),
          .flags = BLE_GATT_CHR_F_WRITE,
          .access_cb = debugLed2 },
        { .uuid = BLE_UUID16_DECLARE(0xFFF2),
          .flags = BLE_GATT_CHR_F_WRITE,
          .access_cb = debugLed3 },
        { 0 } } },

  { /* OTA Service Declaration */
    .type = BLE_GATT_SVC_TYPE_PRIMARY,
    .uuid = BLE_UUID16_DECLARE(0x8018),
    .characteristics =
      (struct ble_gatt_chr_def[]) {
        { /* Receive Firmware Characteristic */
          .uuid = BLE_UUID16_DECLARE(RECV_FW_UUID),
          .access_cb = ble_ota_gatt_handler,
          .val_handle = &receive_fw_val,
          .flags = BLE_GATT_CHR_F_INDICATE | BLE_GATT_CHR_F_WRITE,
        },
        { /* Command Characteristic */
          .uuid = BLE_UUID16_DECLARE(COMMAND_UUID),
          .access_cb = ble_ota_gatt_handler,
          .val_handle = &command_val,
          .flags = BLE_GATT_CHR_F_INDICATE | BLE_GATT_CHR_F_WRITE,
        },

        { 0 } },
  },
  { 0, },
};

/******************************************** Start global functions ********************************************/
void bt_init(void) {
  deviceName = getDeviceName();
  manufacturer = getDeviceManufacturer();
  swVer = getDeviceSwVer();
  hwVer = getDeviceHwVer();

  esp_err_t ret;
  esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();

  ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    nvs_flash_erase();
    nvs_flash_init();
  }
  ble_ota_ringbuf_init(OTA_RINGBUF_SIZE);
  esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT);
  esp_bt_controller_init(&bt_cfg);
  esp_bt_controller_enable(ESP_BT_MODE_BLE);
  esp_ble_ota_host_init();

  esp_ble_ota_recv_fw_data_callback(ota_recv_fw_cb);
}

esp_err_t esp_ble_ota_host_init(void) {
  esp_nimble_init();
  ble_hs_cfg.reset_cb = esp_ble_ota_on_reset;
  ble_hs_cfg.sync_cb = esp_ble_ota_on_sync;
  ble_hs_cfg.gatts_register_cb = gatt_svr_register_cb;
  ble_hs_cfg.store_status_cb = ble_store_util_status_rr;
  ble_hs_cfg.sm_io_cap = BLE_SM_IO_CAP_NO_IO;
  ble_hs_cfg.sm_bonding = 1;
  ble_hs_cfg.sm_our_key_dist = BLE_SM_PAIR_KEY_DIST_ENC;
  ble_hs_cfg.sm_their_key_dist = BLE_SM_PAIR_KEY_DIST_ENC;
  ble_ota_gatt_svr_init();
  ble_svc_gap_device_name_set(deviceName);
  ble_store_config_init();
  nimble_port_freertos_init(esp_ble_ota_host_task);

  return 0;
}

esp_err_t esp_ble_ota_recv_fw_data_callback(esp_ble_ota_recv_fw_cb_t callback) {
  ota_cb_fun_t.recv_fw_cb = callback;
  return ESP_OK;
}

unsigned int esp_ble_ota_get_fw_length(void) {
  return ota_total_len;
}

static void esp_ble_ota_host_task(void *param) {
  /* This function will return only when nimble_port_stop() is executed */
  nimble_port_run();
  nimble_port_freertos_deinit();
}

static void ota_task(void *arg) {
  const esp_partition_t *next_partition = NULL;
  esp_partition_t *partition_ptr = NULL;
  esp_partition_t partition;
  esp_ota_handle_t out_handle = 0;
  uint32_t recv_len = 0;
  uint8_t *data = NULL;
  size_t item_size = 0;

  partition_ptr = (esp_partition_t *)esp_ota_get_boot_partition();
  if (partition_ptr == NULL) {
    vTaskDelete(NULL);
  }
  if (partition_ptr->type != ESP_PARTITION_TYPE_APP) {
    ESP_LOGE(TAG, "esp_current_partition->type != ESP_PARTITION_TYPE_APP\r\n");
    vTaskDelete(NULL);
  }

  if (partition_ptr->subtype == ESP_PARTITION_SUBTYPE_APP_FACTORY) {
    partition.subtype = ESP_PARTITION_SUBTYPE_APP_OTA_0;
  } else {
    next_partition = esp_ota_get_next_update_partition(partition_ptr);
    if (next_partition) {
      partition.subtype = next_partition->subtype;
    } else {
      partition.subtype = ESP_PARTITION_SUBTYPE_APP_OTA_0;
    }
  }
  partition.type = ESP_PARTITION_TYPE_APP;

  partition_ptr = (esp_partition_t *)esp_partition_find_first(partition.type,
      partition.subtype, NULL);
  if (partition_ptr == NULL) {
    ESP_LOGE(TAG, "partition NULL!\r\n");
    vTaskDelete(NULL);
  }

  memcpy(&partition, partition_ptr, sizeof(esp_partition_t));
  if (esp_ota_begin(&partition, OTA_SIZE_UNKNOWN, &out_handle) != ESP_OK) {
    ESP_LOGE(TAG, "esp_ota_begin failed!\r\n");
    vTaskDelete(NULL);
  }

  ESP_LOGI(TAG, "wait for data from ringbuf! fw_len = %u", esp_ble_ota_get_fw_length());
  while (true) {
    data = (uint8_t *)xRingbufferReceive(s_ringbuf, &item_size,
        (TickType_t)portMAX_DELAY);

    // ESP_LOGI(TAG, "recv: %u, recv_total:%"PRIu32"\n", item_size, recv_len + item_size);
    if (item_size != 0) {
      if (esp_ota_write(out_handle, (const void *)data, item_size) != ESP_OK) {
        ESP_LOGE(TAG, "esp_ota_write failed!\r\n");
        vTaskDelete(NULL);
      }
      recv_len += item_size;
      vRingbufferReturnItem(s_ringbuf, (void *)data);

      if (recv_len >= esp_ble_ota_get_fw_length()) {
        break;
      }
    }
  }

  if (esp_ota_end(out_handle) != ESP_OK) {
    ESP_LOGE(TAG, "esp_ota_end failed!\r\n");
    vTaskDelete(NULL);
  }

  if (esp_ota_set_boot_partition(&partition) != ESP_OK) {
    ESP_LOGE(TAG, "esp_ota_set_boot_partition failed!\r\n");
    vTaskDelete(NULL);
  }

  esp_restart();
}

static bool ble_ota_ringbuf_init(uint32_t ringbuf_size) {
  s_ringbuf = xRingbufferCreate(ringbuf_size, RINGBUF_TYPE_BYTEBUF);
  if (s_ringbuf == NULL) {
    return false;
  }

  return true;
}

static size_t write_to_ringbuf(const uint8_t *data, size_t size) {
  BaseType_t done = xRingbufferSend(s_ringbuf, (void *)data, size, (TickType_t)portMAX_DELAY);

  if (done) {
    return size;
  } else {
    return 0;
  }
}

static void ota_recv_fw_cb(uint8_t *buf, uint32_t length) {
  write_to_ringbuf(buf, length);
}

static void esp_ble_ota_write_chr(struct os_mbuf *om) {
  esp_ble_ota_char_t ota_char = find_ota_char_and_desr_by_handle(attribute_handle);

  uint8_t cmd_ack[CMD_ACK_LENGTH] = { 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  uint16_t crc16;

  if ((om->om_data[0] + (om->om_data[1] * 256)) != cur_sector) {
    if ((om->om_data[0] == 0xff) && (om->om_data[1] == 0xff)) {
      ESP_LOGD(TAG, "Last sector");
    } else {
      ESP_LOGE(TAG, "%s - sector index error, cur: %" PRIu32 ", recv: %d",
          __func__, cur_sector, (om->om_data[0] + (om->om_data[1] * 256)));
      cmd_ack[0] = om->om_data[0];
      cmd_ack[1] = om->om_data[1];
      cmd_ack[2] = 0x02; //sector index error
      cmd_ack[3] = 0x00;
      cmd_ack[4] = cur_sector & 0xff;
      cmd_ack[5] = (cur_sector & 0xff00) >> 8;
      crc16 = crc16_ccitt(cmd_ack, 18);
      cmd_ack[18] = crc16 & 0xff;
      cmd_ack[19] = (crc16 & 0xff00) >> 8;
      esp_ble_ota_notification_data(connection_handle, attribute_handle, cmd_ack, ota_char);
    }
  }

  if (om->om_data[2] != cur_packet) {
    if (om->om_data[2] == 0xff) {
      ESP_LOGD(TAG, "last packet");
      goto write_ota_data;
    } else {
      ESP_LOGE(TAG, "%s - packet index error, cur: %" PRIu32 ", recv: %d", __func__, cur_packet, om->om_data[2]);
    }
  }

write_ota_data:
  memcpy(fw_buf + fw_buf_offset, om->om_data + 3, om->om_len - 3);
  fw_buf_offset += om->om_len - 3;

  if (SLIST_NEXT(om, om_next) != NULL) {
    struct os_mbuf *last;
    last = om;
    while (SLIST_NEXT(last, om_next) != NULL) {
      struct os_mbuf *temp = SLIST_NEXT(last, om_next);
      memcpy(fw_buf + fw_buf_offset, temp->om_data, temp->om_len);
      fw_buf_offset += temp->om_len;
      last = SLIST_NEXT(last, om_next);
      temp = NULL;
    }
  }

  ESP_LOGD(TAG, "DEBUG: Sector:%" PRIu32 ", total length:%" PRIu32 ", length:%d", cur_sector,
      fw_buf_offset, om->om_len - 3);
  if (om->om_data[2] == 0xff) {
    resetCurrentPacket();
    cur_sector++;
    ESP_LOGD(TAG, "DEBUG: recv %" PRIu32 " sector", cur_sector);
    goto sector_end;
  } else {
    ESP_LOGD(TAG, "DEBUG: wait next packet");
    cur_packet++;
  }
  return;

sector_end:
  if (fw_buf_offset < ota_block_size) {
    esp_ble_ota_recv_fw_handler(fw_buf, fw_buf_offset);
  } else {
    esp_ble_ota_recv_fw_handler(fw_buf, 4096);
  }

  fw_buf_offset = 0;
  memset(fw_buf, 0x0, ota_block_size);

  cmd_ack[0] = om->om_data[0];
  cmd_ack[1] = om->om_data[1];
  cmd_ack[2] = 0x00;
  cmd_ack[3] = 0x00;
  crc16 = crc16_ccitt(cmd_ack, 18);
  cmd_ack[18] = crc16 & 0xff;
  cmd_ack[19] = (crc16 & 0xff00) >> 8;
  esp_ble_ota_notification_data(connection_handle, attribute_handle, cmd_ack, ota_char);
}

static uint16_t crc16_ccitt(const unsigned char *buf, int len) {
  uint16_t crc16 = 0;
  int32_t i;
  while (len--) {
    crc16 ^= *buf++ << 8;
    for (i = 0; i < 8; i++) {
      if (crc16 & 0x8000) {
        crc16 = (crc16 << 1) ^ 0x1021;
      } else {
        crc16 = crc16 << 1;
      }
    }
  }

  return crc16;
}

static esp_err_t esp_ble_ota_recv_fw_handler(uint8_t *buf, uint32_t length) {
  if (ota_cb_fun_t.recv_fw_cb) {
    ota_cb_fun_t.recv_fw_cb(buf, length);
  }

  return ESP_OK;
}

static void ble_ota_start_write_chr(struct os_mbuf *om) {
  uint8_t cmd_ack[CMD_ACK_LENGTH] = { 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  uint16_t crc16;

  esp_ble_ota_char_t ota_char = find_ota_char_and_desr_by_handle(attribute_handle);

  if ((om->om_data[0] == 0x01) && (om->om_data[1] == 0x00)) {
    start_ota = true;

    ota_total_len = (om->om_data[2]) + (om->om_data[3] * 256) +
      (om->om_data[4] * 256 * 256) + (om->om_data[5] * 256 * 256 * 256);

    /*  Don't forget to implement the state transition */
    /***************************************************/
    /***** State transition to OTA would fit here*******/
    /***************************************************/

    /* Currently, there is no method that deletes the OTA task e.g. after failure */
    xTaskCreate(&ota_task, "ota_task", OTA_TASK_SIZE, NULL, 5, NULL);

    ESP_LOGI(TAG, "recv ota start cmd, fw_length = %" PRIu32 "", ota_total_len);

    fw_buf = (uint8_t *)malloc(ota_block_size * sizeof(uint8_t));
    if (fw_buf == NULL) {
      ESP_LOGE(TAG, "%s -  malloc fail", __func__);
    } else {
      memset(fw_buf, 0x0, ota_block_size);
    }
    cmd_ack[2] = 0x01;
    cmd_ack[3] = 0x00;
    crc16 = crc16_ccitt(cmd_ack, 18);
    cmd_ack[18] = crc16 & 0xff;
    cmd_ack[19] = (crc16 & 0xff00) >> 8;
    esp_ble_ota_notification_data(connection_handle, attribute_handle, cmd_ack,
        ota_char);
  } else if ((om->om_data[0] == 0x02) && (om->om_data[1] == 0x00)) {
    printf("\nCMD_CHAR -> 0 : %d, 1 : %d\r\n", om->om_data[0], om->om_data[1]);
    start_ota = false;
    ota_total_len = 0;
    ESP_LOGI(TAG, "recv ota stop cmd");
    cmd_ack[2] = 0x02;
    cmd_ack[3] = 0x00;
    crc16 = crc16_ccitt(cmd_ack, 18);
    cmd_ack[18] = crc16 & 0xff;
    cmd_ack[19] = (crc16 & 0xff00) >> 8;
    esp_ble_ota_notification_data(connection_handle, attribute_handle, cmd_ack,
        ota_char);
    free(fw_buf);
    fw_buf = NULL;
  }
}

static void gatt_svr_register_cb(struct ble_gatt_register_ctxt *ctxt, void *arg) {
  char buf[BLE_UUID_STR_LEN];

  switch (ctxt->op) {
    case BLE_GATT_REGISTER_OP_SVC:
      ESP_LOGD(TAG, "registered service %s with handle=%d\n", ble_uuid_to_str(ctxt->svc.svc_def->uuid, buf), ctxt->svc.handle);
      break;

    case BLE_GATT_REGISTER_OP_CHR:
      ESP_LOGD(TAG,
          "registering characteristic %s with def_handle=%d val_handle=%d\n",
          ble_uuid_to_str(ctxt->chr.chr_def->uuid, buf), ctxt->chr.def_handle,
          ctxt->chr.val_handle);
      break;

    case BLE_GATT_REGISTER_OP_DSC:
      ESP_LOGD(TAG, "registering descriptor %s with handle=%d\n", ble_uuid_to_str(ctxt->dsc.dsc_def->uuid, buf), ctxt->dsc.handle);
      break;

    default:
      assert(0);
      break;
  }
}

static int ble_ota_gatt_handler(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg) {
  esp_ble_ota_char_t ota_char;
  attribute_handle = attr_handle;

  switch (ctxt->op) {
    case BLE_GATT_ACCESS_OP_READ_CHR:
      ota_char = find_ota_char_and_desr_by_handle(attr_handle);
      ESP_LOGI(TAG, "client read, ota_char: %d", ota_char);
      break;

    case BLE_GATT_ACCESS_OP_WRITE_CHR:
      ota_char = find_ota_char_and_desr_by_handle(attr_handle);
      ESP_LOGD(TAG, "client write; len = %d", ctxt->om->om_len);

      if (ota_char == RECV_FW_CHAR) {
        if (start_ota) {
          esp_ble_ota_write_chr(ctxt->om);

        } else {
          ESP_LOGE(TAG, "%s -  don't receive the start cmd, received: %d", __func__, ota_char);
        }
      } else if (ota_char == CMD_CHAR) {
        ble_ota_start_write_chr(ctxt->om);
      }
      break;

    default:
      return BLE_ATT_ERR_UNLIKELY;
  }
  return 0;
}

static esp_ble_ota_char_t find_ota_char_and_desr_by_handle(uint16_t handle) {
  esp_ble_ota_char_t ret = INVALID_CHAR;

  for (int i = 0; i < OTA_IDX_NB; i++) {
    if (handle == ota_handle_table[i]) {
      switch (i) {
        case RECV_FW_CHAR_VAL_IDX:
          ret = RECV_FW_CHAR;
          break;

        case CMD_CHAR_VAL_IDX:
          ret = CMD_CHAR;
          break;

        default:
          ret = INVALID_CHAR;
          break;
      }
    }
  }
  return ret;
}

static int esp_ble_ota_notification_data(uint16_t conn_handle, uint16_t attr_handle, uint8_t cmd_ack[], esp_ble_ota_char_t ota_char) {
  struct os_mbuf *txom;
  bool notify_enable = false;
  int rc;
  txom = ble_hs_mbuf_from_flat(cmd_ack, CMD_ACK_LENGTH);

  switch (ota_char) {
    case RECV_FW_CHAR:
      if (ota_notification.recv_fw_ntf_enable) {
        notify_enable = true;
      }
      break;

    case CMD_CHAR:
      if (ota_notification.command_ntf_enable) {
        notify_enable = true;
      }
      break;

    case INVALID_CHAR:
      /*default:*/
      break;
  }

  if (notify_enable) {
    rc = ble_gattc_notify_custom(conn_handle, attr_handle, txom);
    if (rc == 0) {
      ESP_LOGD(TAG, "Notification sent, attr_handle = %d", attr_handle);
    } else {
      ESP_LOGE(TAG, "Error in sending notification, rc = %d", rc);
    }
    return rc;
  }

  /* If notifications are disabled return ESP_FAIL */
  ESP_LOGI(TAG, "Notify is disabled");
  return ESP_FAIL;
}

static void esp_ble_ota_fill_handle_table(void) {
  ota_handle_table[RECV_FW_CHAR] = receive_fw_val;
  ota_handle_table[CMD_CHAR] = command_val;
}

static void print_addr(const void *addr) {
  const uint8_t *u8p;
  u8p = addr;
  ESP_LOGI(TAG, "%02x:%02x:%02x:%02x:%02x:%02x", u8p[5], u8p[4], u8p[3], u8p[2], u8p[1], u8p[0]);
}

static void esp_ble_ota_print_conn_desc(struct ble_gap_conn_desc *desc) {
  ESP_LOGI(TAG, "handle=%d our_ota_addr_type=%d our_ota_addr=", desc->conn_handle,
      desc->our_ota_addr.type);
  print_addr(desc->our_ota_addr.val);
  ESP_LOGI(TAG, " our_id_addr_type=%d our_id_addr=", desc->our_id_addr.type);
  print_addr(desc->our_id_addr.val);
  ESP_LOGI(TAG, " peer_ota_addr_type=%d peer_ota_addr=", desc->peer_ota_addr.type);
  print_addr(desc->peer_ota_addr.val);
  ESP_LOGI(TAG, " peer_id_addr_type=%d peer_id_addr=", desc->peer_id_addr.type);
  print_addr(desc->peer_id_addr.val);
  ESP_LOGI(TAG,
      " conn_itvl=%d conn_latency=%d supervision_timeout=%d "
      "encrypted=%d authenticated=%d bonded=%d\n",
      desc->conn_itvl, desc->conn_latency, desc->supervision_timeout,
      desc->sec_state.encrypted, desc->sec_state.authenticated, desc->sec_state.bonded);
}

static void esp_ble_ota_advertise(void) {
  struct ble_gap_adv_params adv_params;
  struct ble_hs_adv_fields fields;
  const char *name;
  int rc;

  memset(&fields, 0, sizeof fields);
  fields.flags = BLE_HS_ADV_F_DISC_GEN | BLE_HS_ADV_F_BREDR_UNSUP;
  fields.tx_pwr_lvl_is_present = 1;
  fields.tx_pwr_lvl = BLE_HS_ADV_TX_PWR_LVL_AUTO;

  name = ble_svc_gap_device_name();
  fields.name = (uint8_t *)name;
  fields.name_len = strlen(name);
  fields.name_is_complete = 1;

  fields.uuids16 = (ble_uuid16_t[]){ BLE_UUID16_INIT(GATT_SVR_SVC_ALERT_UUID) };
  fields.num_uuids16 = 1;
  fields.uuids16_is_complete = 1;

  rc = ble_gap_adv_set_fields(&fields);
  if (rc != 0) {
    ESP_LOGE(TAG, "error setting advertisement data; rc=%d\n", rc);
    return;
  }

  /* Begin advertising. */
  memset(&adv_params, 0, sizeof adv_params);
  adv_params.conn_mode = BLE_GAP_CONN_MODE_UND;
  adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN;
  rc = ble_gap_adv_start(own_addr_type, NULL, BLE_HS_FOREVER, &adv_params,
      esp_ble_ota_gap_event, NULL);
  if (rc != 0) {
    ESP_LOGE(TAG, "error enabling advertisement; rc=%d\n", rc);
    return;
  }
}

static int esp_ble_ota_gap_event(struct ble_gap_event *event, void *arg) {
  struct ble_gap_conn_desc desc;
  int rc;
  esp_ble_ota_char_t ota_char;

  switch (event->type) {
    case BLE_GAP_EVENT_CONNECT:
      /* A new connection was established or a connection attempt failed. */
      ESP_LOGI(TAG, "connection %s; status=%d ",
          event->connect.status == 0 ? "established" : "failed",
          event->connect.status);
      if (event->connect.status == 0) {
        rc = ble_gap_conn_find(event->connect.conn_handle, &desc);
        assert(rc == 0);
        esp_ble_ota_print_conn_desc(&desc);
        connection_handle = event->connect.conn_handle;
      } else {
        /* Connection failed; resume advertising. */
        esp_ble_ota_advertise();
      }
      esp_ble_ota_fill_handle_table();
      return 0;

    case BLE_GAP_EVENT_DISCONNECT:
      ESP_LOGI(TAG, "disconnect; reason=%d ", event->disconnect.reason);
      esp_ble_ota_print_conn_desc(&event->disconnect.conn);

      /* Connection terminated; resume advertising. */
      resetCurrentPacket();
      resetCurrentSector();
      esp_ble_ota_advertise();
      return 0;

    case BLE_GAP_EVENT_CONN_UPDATE:
      /* The central has updated the connection parameters. */
      ESP_LOGI(TAG, "connection updated; status=%d ", event->conn_update.status);
      rc = ble_gap_conn_find(event->conn_update.conn_handle, &desc);
      assert(rc == 0);
      esp_ble_ota_print_conn_desc(&desc);
      return 0;

    case BLE_GAP_EVENT_ADV_COMPLETE:
      ESP_LOGI(TAG, "advertise complete; reason=%d", event->adv_complete.reason);
      esp_ble_ota_advertise();
      return 0;

    case BLE_GAP_EVENT_ENC_CHANGE:
      /* Encryption has been enabled or disabled for this connection. */
      ESP_LOGI(TAG, "encryption change event; status=%d ", event->enc_change.status);
      rc = ble_gap_conn_find(event->enc_change.conn_handle, &desc);
      assert(rc == 0);
      esp_ble_ota_print_conn_desc(&desc);
      return 0;

    case BLE_GAP_EVENT_SUBSCRIBE:
      ota_char = find_ota_char_and_desr_by_handle(event->subscribe.attr_handle);
      ESP_LOGI(TAG, "client subscribe ble_gap_event, ota_char: %d", ota_char);
      ESP_LOGI(TAG,
          "subscribe event; conn_handle=%d attr_handle=%d "
          "reason=%d prevn=%d curn=%d previ=%d curi=%d\n",
          event->subscribe.conn_handle, event->subscribe.attr_handle,
          event->subscribe.reason, event->subscribe.prev_notify,
          event->subscribe.cur_notify, event->subscribe.prev_indicate,
          event->subscribe.cur_indicate);

      switch (ota_char) {
        case RECV_FW_CHAR:
          ota_notification.recv_fw_ntf_enable = true;
          break;

        case CMD_CHAR:
          ota_notification.command_ntf_enable = true;
          break;

        case INVALID_CHAR:
        default:
          break;
      }
      return 0;

    case BLE_GAP_EVENT_MTU:
      ESP_LOGI(TAG, "mtu update event; conn_handle=%d cid=%d mtu=%d\n",
          event->mtu.conn_handle, event->mtu.channel_id, event->mtu.value);
      return 0;

    case BLE_GAP_EVENT_REPEAT_PAIRING:
      rc = ble_gap_conn_find(event->repeat_pairing.conn_handle, &desc);
      assert(rc == 0);
      ble_store_util_delete_peer(&desc.peer_id_addr);

      return BLE_GAP_REPEAT_PAIRING_RETRY;

    case BLE_GAP_EVENT_PASSKEY_ACTION:
      ESP_LOGI(TAG, "PASSKEY_ACTION_EVENT started \n");
      return 0;

    default:
      ESP_LOGI(TAG, "UNHANDLED Event type: %d", event->type);
      break;
  }

  return 0;
}

static int ble_ota_gatt_svr_init(void) {
  int rc;

  ble_svc_gap_init();
  ble_svc_gatt_init();

  rc = ble_gatts_count_cfg(ota_gatt_db);
  if (rc != 0) {
    return rc;
  }

  rc = ble_gatts_add_svcs(ota_gatt_db);
  if (rc != 0) {
    return rc;
  }

  return 0;
}

static void esp_ble_ota_on_reset(int reason) {
  ESP_LOGE(TAG, "Resetting state; reason=%d\n", reason);
}

static void esp_ble_ota_on_sync(void) {
  int rc;

  rc = ble_hs_util_ensure_addr(0);
  assert(rc == 0);

  /* Figure out address to use while advertising (no privacy for now) */
  rc = ble_hs_id_infer_auto(0, &own_addr_type);
  if (rc != 0) {
    ESP_LOGE(TAG, "error determining address type; rc=%d\n", rc);
    return;
  }

  /* Printing ADDR */
  uint8_t addr_val[6] = { 0 };
  rc = ble_hs_id_copy_addr(own_addr_type, addr_val, NULL);

  ESP_LOGI(TAG, "Device Address: ");
  print_addr(addr_val);
  /* Begin advertising. */
  esp_ble_ota_advertise();
}

static int deviceReadMfr(uint16_t conHandle, uint16_t attrHandle, struct ble_gatt_access_ctxt *ctxt, void *arg) {
  os_mbuf_append(ctxt->om, manufacturer, strlen(manufacturer));
  return 0;
}

static int deviceReadSwVer(uint16_t conHandle, uint16_t attrHandle, struct ble_gatt_access_ctxt *ctxt, void *arg) {
  os_mbuf_append(ctxt->om, swVer, strlen(swVer));

  /* If we can check firmware, then we can establish a connection and the software is considererd sane */
  ESP_LOGI(TAG, "SW ver read, marked SW as \"sane\"");
  esp_ota_mark_app_valid_cancel_rollback();
  return 0;
}

static int deviceReadHwVer(uint16_t conHandle, uint16_t attrHandle, struct ble_gatt_access_ctxt *ctxt, void *arg) {
  os_mbuf_append(ctxt->om, hwVer, strlen(hwVer));
  return 0;
}

static void resetCurrentPacket(void) {
  cur_packet = 0;
}

static void resetCurrentSector(void) {
  cur_sector = 0;
}

/********************************************* End local functions *********************************************/

