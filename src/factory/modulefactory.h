#ifndef MODULEFACTORY_H
#define MODULEFACTORY_H

#include <Arduino.h>
#include "../configuration/default/configuration.h"
#include "../configuration/base/configbase.h"
#include "../configuration/base/default_configbase.h"
#include "../enums/configurations.h"
#include "../fmv.h"

#include "../modules/modules.hpp"

#include "../logs/logger.h"


class Modulefactory{
public:

  static Modulebase* getModuleInstance(byte id, Configurations configuration, FMV *fmv)
  {
      Modulebase* mod = nullptr;
      switch(configuration) {
      case Configurations::DEAD_MAN_SWITCH_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance DEAD_MAN_SWITCH_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Dead_man_switch(id, fmv);
        break;
      case Configurations::NEOPIXELS_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance NEOPIXELS_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Neopixels(id, fmv);
        break;
      case Configurations::NTP_TIMESERVICE_CONFIG:
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance NTP_TIMESERVICE_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Ntp_timeservice(id, fmv);
        break;
      case Configurations::CURRENT_MONITOR_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance CURRENT_MONITOR_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Current_monitor(id, fmv);
        break;
      case Configurations::VOLTAGE_MONITOR_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance VOLTAGE_MONITOR_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Voltage_monitor(id, fmv);
        break;
      case Configurations::TEMPERATURE_MONITOR_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance TEMPERATURE_MONITOR_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Temperature_monitor(id, fmv);
        break;
      case Configurations::EXPONENTIAL_POWER_MODULATION_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance EXPONENTIAL_POWER_MODULATION_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Exponential_power_modulation(id, fmv);
        break;
      case Configurations::SERIAL_CONTROL_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance SERIAL_CONTROL_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Serial_control(id, fmv);
        break;
      case Configurations::FORCE_CONTROL_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance FORCE_CONTROL_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Force_control(id, fmv);
        break;
      case Configurations::JOYSTICK_CONTROL_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance JOYSTICK_CONTROL_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Joystick_control(id, fmv);
        break;
      case Configurations::POWER_LIMIT_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance POWER_LIMIT_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Power_limit(id, fmv);
        break;
      case Configurations::SPIFFS_DRIVELOG_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance SPIFFS_DRIVELOG_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Spiffs_drivelog(id, fmv);
        break;
      case Configurations::WIFI_CONNECTION_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance WIFI_CONNECTION_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Wifi_connection(id, fmv);
        break;
      case Configurations::WIFI_SIMPLE_CONTROL_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance WIFI_SIMPLE_CONTROL_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Wifi_simple_control(id, fmv);
        break;
      case Configurations::VESC_CONTROLLER_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance VESC_CONTROLLER_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Vesc_controller(id, fmv);
        break;
      case Configurations::PWM_STEERING_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance PWM_STEERING_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Pwm_steering(id, fmv);
        break;
      case Configurations::WEB_UPDATE_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance WEB_UPDATE_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Web_update(id, fmv);
        break;
      case Configurations::PWM_CONTROLLER_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance PWM_CONTROLLER_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Pwm_controller(id, fmv);
        break;
      case Configurations::WEBSOCKET_CONTROL_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance WEBSOCKET_CONTROL_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Websocket_control(id, fmv);
        break;
      case Configurations::CONTROL_PRIORITY_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance CONTROL_PRIORITY_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Control_priority(id, fmv);
        break;
      case Configurations::ARDUINO_OTA_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance ARDUINO_OTA_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Arduino_ota(id, fmv);
        break;
      case Configurations::WEBSOCKET_CONNECTION_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance WEBSOCKET_CONNECTION_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Websocket_connection(id, fmv);
        break;
      case Configurations::WEBSOCKET_DRIVELOG_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getModuleInstance WEBSOCKET_DRIVELOG_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        mod = new Websocket_drivelog(id, fmv);
        break;
      }
      return mod;
  }
};

#endif
