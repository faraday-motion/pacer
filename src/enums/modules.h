#ifndef MODULES_H
#define MODULES_H


enum class Modules {
  NONE = 0,
  DEAD_MAN_SWITCH = 1,
  NEOPIXELS = 2,
  FASTLED = 3,
  SERIAL_CONTROL = 4,
  JOYSTICK_CONTROL = 5,
  FORCE_CONTROL = 6,
  POWER_LIMIT = 7,
  CURRENT_MONITOR = 8,
  VOLTAGE_MONITOR = 9,
  TEMPERATURE_MONITOR = 10,
  DIRECT_POWER_MODULATION = 11,
  EXPONENTIAL_POWER_MODULATION = 12,
  WIFI_CONNECTION = 13,
  WIFI_SIMPLE_CONTROL = 14,
  BT_CONNECTION = 15,
  WEB_UPDATE = 16,
  SPIFFS_DRIVELOG = 17,
  VESC_CONTROLLER = 18,
  PWM_STEERING = 19,
  PWM_CONTROLLER = 20,
  WEBSOCKET_CONTROL = 21,
  CONTROL_PRIORITY = 22,
  NTP_TIMESERVICE = 23,
  ARDUINO_OTA = 24,
  WEBSOCKET_CONNECTION = 25,
  WEBSOCKET_DRIVELOG = 26
};

#endif