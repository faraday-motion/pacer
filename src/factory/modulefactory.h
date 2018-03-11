#ifndef MODULEFACTORY_H
#define MODULEFACTORY_H

#include <Arduino.h>
#include "../configuration/default/configuration.h"
#include "../configuration/base/configbase.h"
#include "../configuration/base/default_configbase.h"
#include "../enums/configurations.h"

#include "../interfaces/interfaces.hpp"
#include "../modules/modules.hpp"

#include "../log/logger.h"


class Modulefactory{

private:

public:


  static Modulebase * getModuleInstance(byte id, int configuration, IFMV * fmv)
  {
      Modulebase * mod = nullptr;
      switch(configuration) {
      case Configurations::NEOPIXELS_LED_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance NEOPIXELS_LED_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Neopixels_led(id, fmv);
        //mod = static_cast<IModule*>(new Neopixels_led(id, fmv));
        break;
      case Configurations::NTP_TIMESERVICE_CONFIG:
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance NTP_TIMESERVICE_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Ntp_timeservice(id, fmv);
//        mod = static_cast<IModule*>(new Ntp_timeservice(id, fmv));
        break;
      case Configurations::CURRENT_MONITOR_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance CURRENT_MONITOR_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Current_monitor(id, fmv);
//        mod = static_cast<IModule*>(new Current_monitor(id, fmv));
      break;
      case Configurations::VOLTAGE_MONITOR_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance VOLTAGE_MONITOR_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Voltage_monitor(id, fmv);
//        mod = static_cast<IModule*>(new Voltage_monitor(id, fmv));
      break;
      case Configurations::TEMPERATURE_MONITOR_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance TEMPERATURE_MONITOR_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Temperature_monitor(id, fmv);
  //      mod = static_cast<IModule*>(new Temperature_monitor(id, fmv));
        break;
      case Configurations::EXPONENTIAL_POWER_MODULATION_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance EXPONENTIAL_POWER_MODULATION_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Exponential_power_modulation(id, fmv);
      //  mod = static_cast<IModule*>(new Exponential_power_modulation(id, fmv));
        break;
      case Configurations::SERIAL_CONTROL_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance SERIAL_CONTROL_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Serial_control(id, fmv);
      //  IControlModule * icm = static_cast<IControlModule*>(new Serial_control(id, fmv));
        //mod = static_cast<IModule*>(icm);
        break;
      case Configurations::FORCE_CONTROL_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance FORCE_CONTROL_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Force_control(id, fmv);
      //  mod = static_cast<IModule*>(new Force_control(id, fmv));
        break;
      case Configurations::JOYSTICK_CONTROL_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance JOYSTICK_CONTROL_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Joystick_control(id, fmv);
    //    mod = static_cast<IModule*>(new Joystick_control(id, fmv));
        break;
      case Configurations::POWER_LIMIT_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance POWER_LIMIT_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Power_limit(id, fmv);
    //    mod = static_cast<IModule*>(new Power_limit(id, fmv));
        break;
      case Configurations::SPIFFS_DRIVELOG_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance SPIFFS_DRIVELOG_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Spiffs_drivelog(id, fmv);
      //  mod = static_cast<IModule*>(new Spiffs_drivelog(id, fmv));
        break;
      case Configurations::WIFI_CONNECTION_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance WIFI_CONNECTION_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Wifi_connection(id, fmv);
      //  mod = static_cast<IModule*>(new Wifi_connection(id, fmv));
        break;
      case Configurations::WIFI_SIMPLE_CONTROL_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance WIFI_SIMPLE_CONTROL_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Wifi_simple_control(id, fmv);
    //    mod = static_cast<IModule*>(new Wifi_simple_control(id, fmv));
        break;
      case Configurations::VESC_CONTROLLER_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance VESC_CONTROLLER_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Vesc_controller(id, fmv);
      //  mod = static_cast<IModule*>(new Vesc_controller(id, fmv));
        break;
      case Configurations::PWM_STEERING_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance PWM_STEERING_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Pwm_steering(id, fmv);
      //  mod = static_cast<IModule*>(new Pwm_steering(id, fmv));
        break;
      case Configurations::WEB_UPDATE_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance WEB_UPDATE_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Web_update(id, fmv);
      //  mod = static_cast<IModule*>(new Web_update(id, fmv));
        break;
      case Configurations::PWM_CONTROLLER_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance PWM_CONTROLLER_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Pwm_controller(id, fmv);
    //    mod = static_cast<IModule*>(new Pwm_controller(id, fmv));
        break;
      case Configurations::WEBSOCKET_CONTROL_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance WEBSOCKET_CONTROL_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Websocket_control(id, fmv);
      //  mod = static_cast<IModule*>(new Websocket_control(id, fmv));
        break;
      case Configurations::CONTROL_PRIORITY_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance CONTROL_PRIORITY_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Control_priority(id, fmv);
    //    mod = static_cast<IModule*>(new Control_priority(id, fmv));
        break;
      case Configurations::ARDUINO_OTA_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance ARDUINO_OTA_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Arduino_ota(id, fmv);
      //  mod = static_cast<IModule*>(new Arduino_ota(id, fmv));
        break;
      case Configurations::WEBSOCKET_CONNECTION_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance WEBSOCKET_CONNECTION_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Websocket_connection(id, fmv);
//        mod = static_cast<IModule*>(new Websocket_connection(id, fmv));
        break;
      case Configurations::WEBSOCKET_DRIVELOG_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance WEBSOCKET_DRIVELOG_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Websocket_drivelog(id, fmv);
  //      mod = static_cast<IModule*>(new Websocket_drivelog(id, fmv));
        break;
      case Configurations::MQTT_CLIENT_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance MQTT_CLIENT_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Mqtt_client(id, fmv);
    //    mod = static_cast<IModule*>(new Mqtt_client(id, fmv));
        break;
      case Configurations::ESP32_DIGITAL_LED_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance ESP32_DIGITAL_LED_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Esp32_digital_led(id, fmv);
      //  mod = static_cast<IModule*>(new Esp32_digital_led(id, fmv));
        break;
      case Configurations::ANALOG_SENSOR_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance ANALOG_SENSOR_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Analog_sensor(id, fmv);
        //mod = static_cast<IModule*>(new Analog_sensor(id, fmv));
        break;
      case Configurations::BOOLEAN_SENSOR_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance BOOLEAN_SENSOR_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Boolean_sensor(id, fmv);
//        mod = static_cast<IModule*>(new Boolean_sensor(id, fmv));
        break;
      case Configurations::SERIAL_LOG_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance SERIAL_LOG_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Serial_log(id, fmv);
  //      mod = static_cast<IModule*>(new Serial_log(id, fmv));
        break;
      case Configurations::SPIFFS_LOG_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance SPIFFS_LOG_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Spiffs_log(id, fmv);
    //    mod = static_cast<IModule*>(new Spiffs_log(id, fmv));
        break;
      case Configurations::WEBSOCKET_SERVER_LOG_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance WEBSOCKET_SERVER_LOG_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Websocket_server_log(id, fmv);
        //mod = static_cast<IModule*>(new Websocket_server_log(id, fmv));
        break;
      }
      return mod;
  }
};

#endif
