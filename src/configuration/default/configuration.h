#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "../../macros.h"
#include "./default_pins.h"

//Configuration of all features
#define BAUDRATE                      115200
#define FORMATONFAILURE               true
#define JSONBUFFERSIZE                512
#define EXTERNALCOMMANDS_LIMIT        80
#define SPIFF_LOG_FILENAME            "/logfile.txt"
#define NEOPIXELS_PIXELS              12
#define FASTLED_PIXELS                12
#define FEATURE_VOLTAGE_MONITOR       1
#define DEFAULTSMOOTHALPHA            0.5
#define WIFI_CONNECTION_HOST          "fmwebupdate"
#define WIFI_CONNECTION_SSID          "c12345"
#define WIFI_CONNECTION_PASSWORD      "6BF87898C6A5"
#define WIFI_CONNECTION_AP_SSID       "FARADAY009"
#define WIFI_CONNECTION_AP_PASSWORD   "faraday009"
#define WIFI_CONNECTION_AP_CHANNEL    11
#define SPIFF_DRIVELOG_FILENAME       "/drivelog.txt"
#define NTP_TIME_SERVER               "pool.ntp.org"
#define NTP_TIME_TIMEZONE             "CET-1CEST-2,M3.5.0/2,M10.5.0/3"
#define NTP_DATETIME_FORMAT           "%Y-%m-%d %H:%M:%S"
#define MODULE_DEFAULT_ENABLED        true
#define LOGGER_LOG_DEFALT_ENABLED     true
#define LOGGER_LOG_MAXBUFFER          50
#define WEBSOCKET_PORT                81

#endif
