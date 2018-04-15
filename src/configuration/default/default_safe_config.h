#ifndef DEFAULT_SAFE_CONFIG_H
#define DEFAULT_SAFE_CONFIG_H

#include <vector>
#include <Arduino.h>
#include "../base/default_configbase.h"
#include "./configuration.h"
#include "../configurator.h"
#include "../../enums/loglevel.h"
#include "../../modules/modules.hpp"
#include "../wheel_config.h"

class Default_safe_config : public Default_configbase{
private:

public:

  Default_safe_config() : Default_configbase() {

  }

  virtual void addConfigs()
  {
      byte id = 0;

      //0
      Serial_log_config * cfg0 = new Serial_log_config(id++);
      cfg0 -> logLevel = LogLevel::WARNING;
      cfg0 -> enabled = true;
      Configurator::Instance().addConfig(cfg0);

      //13
      Wifi_connection_config* cfg13 = new Wifi_connection_config(id++);
      cfg13 -> enabled = true;
      Configurator::Instance().addConfig(cfg13);

      //14
      Wifi_simple_control_config* cfg14 = new Wifi_simple_control_config(id++);
      cfg14 -> enabled = true;
      Configurator::Instance().addConfig(cfg14);

      //10
      Exponential_power_modulation_config * cfg10 = new Exponential_power_modulation_config(id++);
      cfg10 -> enabled = true;
      Configurator::Instance().addConfig(cfg10);

      //11
      Power_limit_config * cfg11 = new Power_limit_config(id++);
      cfg11 -> enabled = true;
      cfg11 -> deadSensorName = "";
      Configurator::Instance().addConfig(cfg11);

      //18
      Web_update_config * cfg18 = new Web_update_config(id++);
      cfg18 -> enabled = true;
      Configurator::Instance().addConfig(cfg18);

      //25
      Websocket_connection_config * cfg25 = new Websocket_connection_config(id++);
      cfg25 -> enabled = true;
      Configurator::Instance().addConfig(cfg25);

      //19
      Websocket_control_config * cfg19 = new Websocket_control_config(id++);
      cfg19 -> enabled = true;
      Configurator::Instance().addConfig(cfg19);

      //26
      Websocket_drivelog_config * cfg26 = new Websocket_drivelog_config(id++);
      cfg26 -> enabled = true;
      Configurator::Instance().addConfig(cfg26);

      //24
      Arduino_ota_config* cfg24 = new Arduino_ota_config(id++);
      Configurator::Instance().addConfig(cfg24);

      //20
      Control_priority_config * cfg20 = new Control_priority_config(id++);
      cfg20 -> enabled = true;
      cfg20 -> priority1 = cfg14 -> getId(); //Wifi simple
      cfg20 -> priority2 = cfg19 -> getId(); //Websocket
      Configurator::Instance().addConfig(cfg20);
    }
};

#endif
