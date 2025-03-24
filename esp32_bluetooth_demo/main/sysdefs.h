#ifndef __SYSDEFS_H__
#define __SYSDEFS_H__

#include <stdint.h>

#define MANUFACTURER    "TJHooij"
#define DEVICE_NAME     "fota"
#define SW_VER          "v1.0.0"
#define HW_VER          "v1.0.0"

/* This is a placeholder value, currently set to max readable adc value; CALIBRATE */
/*#define THERMAL_SHUTDOWN_SAFETY_VAL 500*/

/* General definitions */
#define MSEC_PER_TICK           (10)
#define MSEC_1000               (1000)
#define MSEC_10                 (10)
#define PERCENT_100             (100)

#define FACTOR_2                (2)

#define ENABLED                 (1)
#define DISABLED                (0)
#define HIGH                    (1)
#define LOW                     (0)

/* System structs */
enum wifiConfigured {
  NOT_CONFIGURED = 0,
  CONFIGURED,

  NO_OF_WIFI_CONFIGS
};

enum BluetoothConnected {
  NOT_CONNECTED = 0,
  CONNECTED,

  NO_OF_BT_CONFIGS
};

struct myDevice {
  const char* deviceName;
  const char* manufacturer;
  const char* swVer;
  const char* hwVer;
};

/* Getters and setters for device information */
const char* getDeviceName(void);
const char* getDeviceManufacturer(void);
const char* getDeviceSwVer(void);
const char* getDeviceHwVer(void);

#endif /* __SYSDEFS_H__ */
