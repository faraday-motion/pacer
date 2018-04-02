#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "../../macros.h"
#include "./default_pins.h"

//Configuration of all features
#define BAUDRATE                      115200
#define FORMATONFAILURE               true
#define JSONBUFFERSIZE                512
#define SPIFF_LOG_FILENAME            "/logfile.txt"
#define NEOPIXELS_PIXELS              12
#define FASTLED_PIXELS                12
#define FEATURE_VOLTAGE_MONITOR       1
#define SMOOTH_ALPHA_POSITIVE         0.1
#define SMOOTH_ALPHA_NEGATIVE         0.5
#define WIFI_CONNECTION_HOST          "fmwebupdate"
#define WIFI_CONNECTION_SSID          "c12345"
#define WIFI_CONNECTION_PASSWORD      "c12345"
#define WIFI_CONNECTION_AP_SSID       "FARADAY009"
#define WIFI_CONNECTION_AP_PASSWORD   "faraday009"
#define WIFI_CONNECTION_AP_CHANNEL    11
#define SPIFF_DRIVELOG_FILENAME       "/drivelog.txt"
#define NTP_TIME_SERVER               "pool.ntp.org"
#define NTP_TIME_TIMEZONE             "CET-1CEST-2,M3.5.0/2,M10.5.0/3"
#define NTP_DATETIME_FORMAT           "%Y-%m-%d %H:%M:%S"
#define MODULE_DEFAULT_ENABLED        true
#define LOGGER_LOG_BUFFER             100
#define WEBSOCKET_PORT                81
#define WEBSOCKET_PING_TIMEOUT        1000
#define WEBSOCKET_EVICT_TIMEOUT       5000
#define WEBSOCKET_PING_PAYLOAD        "ping"
#define ARDUINO_OTA_PORT              3232
#define ARDUINO_OTA_HOSTNAME          "myesp32"
#define ARDUINO_OTA_PASSWORD          ""
#define MQTT_CLIENT_SERVER            "broker.mqtt-dashboard.com"
#define MQTT_CLIENT_PORT              1883

#endif
