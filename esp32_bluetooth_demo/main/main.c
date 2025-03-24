#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>
#include "esp_err.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "freertos/projdefs.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#include "../components/bt_ota.h"
#include "sysdefs.h"
#include "main.h"

const char* TAG = "main";

enum {
  LEVEL_OFF = 0,
  LEVEL_1,
  LEVEL_2,

  MAX_NUM_OF_LEVEL
};

struct Device {
  uint8_t state;
  uint16_t time;
};
struct Device dev = { .state = LEVEL_OFF, .time = 0 };

static uint32_t tickCount = 0;
static bool playing = false;

static esp_err_t mainInit(void);
static esp_err_t mainTickHandler(void);

void app_main(void) {
  ESP_LOGI(TAG, "App A start!");
  ESP_ERROR_CHECK(mainInit());
  bt_init();



  /*main_setTime(10);*/
  /*main_setLevel(LEVEL_1);*/

  while (true) {
    ESP_ERROR_CHECK(mainTickHandler());


    if (!(tickCount % 100)) {
      if (playing) {
        if (dev.time) {
          dev.time--;
          ESP_LOGI(TAG, "Time left: %d.", dev.time);
          if (0 == dev.time) {
            ESP_LOGI(TAG, "Timer ran out, turning off now.");
            playing = false;
            dev.state = LEVEL_OFF;
          }

          if (dev.state) {
            gpio_set_level(27, 1);
          } else {
            gpio_set_level(27, 0);
          }
        }
      }
    }

    tickCount++;
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void main_setTime(uint16_t time) {
    ESP_LOGI(TAG, "Set time to: %d.", time);
    playing = true;
    dev.time = time;
}
void main_setLevel(uint8_t state) {
    ESP_LOGI(TAG, "Set state to: %d.", state);
    switch(state) {
      case LEVEL_OFF:
        dev.state = LEVEL_OFF;
        break;

      case LEVEL_1:
        dev.state = LEVEL_1;
        break;

      case LEVEL_2:
        dev.state = LEVEL_2;
        break;

      case MAX_NUM_OF_LEVEL:
      default:
        break;
    }
}

static esp_err_t mainInit(void) {
  gpio_config_t conf = {
        .pin_bit_mask = (1ULL << 27), // Set the specific pin for the LED
        .mode = GPIO_MODE_OUTPUT,          // Set the pin as output
        .pull_up_en = GPIO_PULLUP_DISABLE, // No pull-up resistor
        .pull_down_en = GPIO_PULLDOWN_DISABLE, // No pull-down resistor
        .intr_type = GPIO_INTR_DISABLE,    // No interrupt
  };
  gpio_config(&conf);
  /*gpio_set_level(27, 1);*/
  gpio_set_level(27, 0);

  return ESP_OK;
}

static esp_err_t mainTickHandler(void) {
  /* 10ms main tick */
  return ESP_OK;
}

const char* getDeviceManufacturer(void) {
  return MANUFACTURER;
}

const char* getDeviceName(void) {
  return DEVICE_NAME;
}

const char* getDeviceSwVer(void) {
  return SW_VER;
}

const char* getDeviceHwVer(void) {
  return HW_VER;
}

