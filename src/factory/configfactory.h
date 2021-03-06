#ifndef CONFIGFACTORY_H
#define CONFIGFACTORY_H

#include "../configuration/default/configuration.h"
#include "../configuration/base/configbase.h"
#include "../configuration/base/default_configbase.h"

#include "../modules/configs.hpp"
#include "../enums/loglevel.h"

#include "../configuration/wheel_config.h"
#include "../log/logger.h"

class Configfactory{
public:

  static Configbase * getConfigInstance(byte id, int configuration)
  {
      Configbase * cfg = nullptr;

      switch(configuration) {
      case Configurations::BOOLEAN_SENSOR_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getConfigInstance BOOLEAN_SENSOR_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        cfg = new Boolean_sensor_config(id);
        break;
      case Configurations::ANALOG_SENSOR_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getConfigInstance ANALOG_SENSOR_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        cfg = new Analog_sensor_config(id);
        break;
      case Configurations::NEOPIXELS_LED_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getConfigInstance NEOPIXELS_LED_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        cfg = new Neopixels_led_config(id);
        break;
      case Configurations::NTP_TIMESERVICE_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getConfigInstance NTP_TIMESERVICE_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        cfg = new Ntp_timeservice_config(id);
        break;
      case Configurations::EXPONENTIAL_POWER_MODULATION_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getConfigInstance EXPONENTIAL_POWER_MODULATION_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        cfg = new Exponential_power_modulation_config(id);
        break;
      case Configurations::SERIAL_CONTROL_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getConfigInstance SERIAL_CONTROL_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        cfg = new Serial_control_config(id);
        break;
      case Configurations::FORCE_CONTROL_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getConfigInstance FORCE_CONTROL_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        cfg = new Force_control_config(id);
        break;
      case Configurations::JOYSTICK_CONTROL_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getConfigInstance JOYSTICK_CONTROL_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        cfg = new Joystick_control_config(id);
        break;
      case Configurations::SPIFFS_DRIVELOG_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getConfigInstance SPIFFS_DRIVELOG_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        cfg = new Spiffs_drivelog_config(id);
        break;
      case Configurations::WEBSOCKET_SERVER_LOG_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getConfigInstance SPIFFS_DRIVELOG_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        cfg = new Websocket_server_log_config(id);
        break;
      case Configurations::WIFI_CONNECTION_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getConfigInstance WIFI_CONNECTION_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        cfg = new Wifi_connection_config(id);
        break;
      case Configurations::WIFI_SIMPLE_CONTROL_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getConfigInstance WIFI_SIMPLE_CONTROL_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        cfg = new Wifi_simple_control_config(id);
        break;
      case Configurations::POWER_LIMIT_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getConfigInstance POWER_LIMIT_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        cfg = new Power_limit_config(id);
        break;
      case Configurations::VESC_CONTROLLER_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getConfigInstance VESC_CONTROLLER_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        cfg = new Vesc_controller_config(id);
        break;
      case Configurations::SERIAL_LOG_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getConfigInstance SERIAL_LOG_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        cfg = new Serial_log_config(id);
        break;
      case Configurations::SPIFFS_LOG_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getConfigInstance SPIFFS_LOG_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        cfg = new Spiffs_log_config(id);
        break;
      case Configurations::WHEEL_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getConfigInstance WHEEL_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        cfg = new Wheel_config(id);
        break;
      case Configurations::PWM_STEERING_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getConfigInstance PWM_STEERING_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        cfg = new Pwm_steering_config(id);
        break;
      case Configurations::WEB_UPDATE_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getConfigInstance WEB_UPDATE_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        cfg = new Web_update_config(id);
        break;
      case Configurations::PWM_CONTROLLER_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getConfigInstance PWM_CONTROLLER_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        cfg = new Pwm_controller_config(id);
        break;
      case Configurations::WEBSOCKET_CONTROL_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getConfigInstance WEBSOCKET_CONTROL_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        cfg = new Websocket_control_config(id);
        break;
      case Configurations::CONTROL_PRIORITY_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getConfigInstance CONTROL_PRIORITY_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        cfg = new Control_priority_config(id);
        break;
      case Configurations::ARDUINO_OTA_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getConfigInstance ARDUINO_OTA_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        cfg = new Arduino_ota_config(id);
        break;
      case Configurations::WEBSOCKET_CONNECTION_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getConfigInstance WEBSOCKET_CONNECTION_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        cfg = new Websocket_connection_config(id);
        break;
      case Configurations::WEBSOCKET_DRIVELOG_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getConfigInstance WEBSOCKET_CONNECTION_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        cfg = new Websocket_drivelog_config(id);
        break;
      case Configurations::MQTT_CONNECTION_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getConfigInstance MQTT_CONNECTION_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        cfg = new Mqtt_connection_config(id);
        break;
      case Configurations::MQTT_CONTROL_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getConfigInstance MQTT_CONTROL_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        cfg = new Mqtt_control_config(id);
        break;
      case Configurations::ESP32_DIGITAL_LED_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getConfigInstance ESP32_DIGITAL_LED_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        cfg = new Esp32_digital_led_config(id);
        break;

      return cfg;
    }
  }
};

#endif
