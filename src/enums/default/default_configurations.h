#ifndef DEFAULT_CONFIGURATIONS_H
#define DEFAULT_CONFIGURATIONS_H

class Default_configurations {
public:
  static const int NONE = 0;
  static const int SWITCH_CONFIG = 1;
  static const int ANALOG_CONFIG = 2;
  static const int DEAD_MAN_SWITCH_CONFIG = 3;
  static const int NEOPIXELS_CONFIG = 4;
  static const int CURRENT_MONITOR_CONFIG = 5;
  static const int VOLTAGE_MONITOR_CONFIG = 6;
  static const int TEMPERATURE_MONITOR_CONFIG = 7;
  static const int EXPONENTIAL_POWER_MODULATION_CONFIG = 8;
  static const int SERIAL_CONTROL_CONFIG = 9;
  static const int FORCE_CONTROL_CONFIG = 10;
  static const int JOYSTICK_CONTROL_CONFIG = 11;
  static const int SPIFFS_DRIVELOG_CONFIG = 12;
  static const int POWER_LIMIT_CONFIG = 13;
  static const int WIFI_CONNECTION_CONFIG = 14;
  static const int SERIAL_LOG_CONFIG = 15;
  static const int SPIFFS_LOG_CONFIG = 16;
  static const int WHEEL_CONFIG = 17;
  static const int VESC_CONTROLLER_CONFIG = 18;
  static const int WIFI_SIMPLE_CONTROL_CONFIG = 19;
  static const int PWM_STEERING_CONFIG = 20;
  static const int PWM_CONTROLLER_CONFIG = 21;
  static const int WEB_UPDATE_CONFIG = 22;
  static const int WEBSOCKET_CONTROL_CONFIG = 23;
  static const int DIRECT_POWER_MODULATION_CONFIG = 24;
  static const int WEBSOCKET_SERVER_LOG_CONFIG = 25;
  static const int CONTROL_PRIORITY_CONFIG = 26;
  static const int NTP_TIMESERVICE_CONFIG = 27;
  static const int ARDUINO_OTA_CONFIG = 28;
  static const int WEBSOCKET_CONNECTION_CONFIG = 29;
  static const int WEBSOCKET_DRIVELOG_CONFIG = 30;
  static const int MQTT_CLIENT_CONFIG = 31;
};

#endif
