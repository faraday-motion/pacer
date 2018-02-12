#ifndef CONFIGURATIONS_H
#define CONFIGURATIONS_H

enum class Configurations {
  NONE = 0,
  SWITCH_CONFIG= 1,
  ANALOG_CONFIG= 2,
  DEAD_MAN_SWITCH_CONFIG= 3,
  NEOPIXELS_CONFIG= 4,
  CURRENT_MONITOR_CONFIG= 5,
  VOLTAGE_MONITOR_CONFIG= 6,
  TEMPERATURE_MONITOR_CONFIG= 7,
  EXPONENTIAL_POWER_MODULATION_CONFIG= 8,
  SERIAL_CONTROL_CONFIG= 9,
  FORCE_CONTROL_CONFIG= 10,
  JOYSTICK_CONTROL_CONFIG= 11,
  SPIFFS_DRIVELOG_CONFIG= 12,
  POWER_LIMIT_CONFIG= 13,
  WIFI_CONNECTION_CONFIG= 14,
  SERIAL_LOG_CONFIG= 15,
  SPIFFS_LOG_CONFIG= 16,
  WHEEL_CONFIG= 17,
  VESC_CONTROLLER_CONFIG= 18,
  WIFI_SIMPLE_CONTROL_CONFIG= 19,
  PWM_STEERING_CONFIG= 20,
  PWM_CONTROLLER_CONFIG= 21,
  WEB_UPDATE_CONFIG= 22,
  WEBSOCKET_CONTROL_CONFIG= 23,
  DIRECT_POWER_MODULATION_CONFIG = 24,
  WEBSOCKET_SERVER_LOG_CONFIG = 25,
  CONTROL_PRIORITY_CONFIG = 26,
  NTP_TIMESERVICE_CONFIG = 27,
  ARDUINO_OTA_CONFIG = 28
};

#endif
