#ifndef DEFAULT_CONFIGURATIONS_H
#define DEFAULT_CONFIGURATIONS_H

#include <Arduino.h>

class Default_configurations {
public:
  static const byte NONE = 0;
  static const byte BOOLEAN_SENSOR_CONFIG = 1;
  static const byte ANALOG_SENSOR_CONFIG = 2;
  static const byte NEOPIXELS_LED_CONFIG = 4;
  static const byte CURRENT_MONITOR_CONFIG = 5;
  static const byte VOLTAGE_MONITOR_CONFIG = 6;
  static const byte TEMPERATURE_MONITOR_CONFIG = 7;
  static const byte EXPONENTIAL_POWER_MODULATION_CONFIG = 8;
  static const byte SERIAL_CONTROL_CONFIG = 9;
  static const byte FORCE_CONTROL_CONFIG = 10;
  static const byte JOYSTICK_CONTROL_CONFIG = 11;
  static const byte SPIFFS_DRIVELOG_CONFIG = 12;
  static const byte POWER_LIMIT_CONFIG = 13;
  static const byte WIFI_CONNECTION_CONFIG = 14;
  static const byte SERIAL_LOG_CONFIG = 15;
  static const byte SPIFFS_LOG_CONFIG = 16;
  static const byte WHEEL_CONFIG = 17;
  static const byte VESC_CONTROLLER_CONFIG = 18;
  static const byte WIFI_SIMPLE_CONTROL_CONFIG = 19;
  static const byte PWM_STEERING_CONFIG = 20;
  static const byte PWM_CONTROLLER_CONFIG = 21;
  static const byte WEB_UPDATE_CONFIG = 22;
  static const byte WEBSOCKET_CONTROL_CONFIG = 23;
  static const byte DIRECT_POWER_MODULATION_CONFIG = 24;
  static const byte WEBSOCKET_SERVER_LOG_CONFIG = 25;
  static const byte CONTROL_PRIORITY_CONFIG = 26;
  static const byte NTP_TIMESERVICE_CONFIG = 27;
  static const byte ARDUINO_OTA_CONFIG = 28;
  static const byte WEBSOCKET_CONNECTION_CONFIG = 29;
  static const byte WEBSOCKET_DRIVELOG_CONFIG = 30;
  static const byte MQTT_CLIENT_CONFIG = 31;
  static const byte ESP32_DIGITAL_LED_CONFIG = 32;
};

#endif
