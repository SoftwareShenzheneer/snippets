| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-C6 | ESP32-H2 | ESP32-S2 | ESP32-S3 |
| ----------------- | XXXXX | -------- | XXXXXXXX | -------- | -------- | -------- | XXXXXXXX |

# OTAU Template

This template has taken BIG inspiration from the esp_ble_ota example (https://github.com/EspressifApps/esp-ble-ota-android) but with some changes that made it easier for me to implement. The intention behind this document is not to commercialize it, but as a personal point of reference. 

It has been tested with ESP32, ESP32-C3, ESP32-C6 and ESP32-S3 chips. Note that currently this does not work on ESP32-C6 chips.
To use this, just copy the project and add bt_ota.c/.h to an existing project. If there is an existing bluetooth implementation, this needs to be integrated.
Please take note that this relies on the use of the NimBLE stack.

