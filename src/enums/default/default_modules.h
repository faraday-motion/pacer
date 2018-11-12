#ifndef DEFAULT_MODULES_H
#define DEFAULT_MODULES_H

#include <Arduino.h>

class Default_modules {
public:
  static const byte NONE = 0;
  static const byte NEOPIXELS_LED = 2;
  static const byte FASTLED_LED = 3;
  static const byte SERIAL_CONTROL = 4;
  static const byte JOYSTICK_CONTROL = 5;
  static const byte FORCE_CONTROL = 6;
  static const byte POWER_LIMIT = 7;
  static const byte CURRENT_MONITOR = 8;
  static const byte VOLTAGE_MONITOR = 9;
  static const byte TEMPERATURE_MONITOR = 10;
  static const byte DIRECT_POWER_MODULATION = 11;
  static const byte EXPONENTIAL_POWER_MODULATION = 12;
  static const byte WIFI_CONNECTION = 13;
  static const byte WIFI_SIMPLE_CONTROL = 14;
  static const byte BT_CONNECTION = 15;
  static const byte WEB_UPDATE = 16;
  static const byte SPIFFS_DRIVELOG = 17;
  static const byte VESC_CONTROLLER = 18;
  static const byte PWM_STEERING = 19;
  static const byte PWM_CONTROLLER = 20;
  static const byte WEBSOCKET_CONTROL = 21;
  static const byte CONTROL_PRIORITY = 22;
  static const byte NTP_TIMESERVICE = 23;
  static const byte ARDUINO_OTA = 24;
  static const byte WEBSOCKET_CONNECTION = 25;
  static const byte WEBSOCKET_DRIVELOG = 26;
  static const byte MQTT_CONNECTION = 27;
  static const byte ESP32_DIGITAL_LED = 28;
  static const byte ANALOG_SENSOR = 29;
  static const byte BOOLEAN_SENSOR = 30;
  static const byte SERIAL_LOG = 31;
  static const byte SPIFFS_LOG = 32;
  static const byte WEBSOCKET_SERVER_LOG = 33;
  static const byte MQTT_CONTROL = 34;


};

#endif
