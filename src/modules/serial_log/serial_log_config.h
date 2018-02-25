#ifndef SERIAL_LOG_CONFIG_H
#define SERIAL_LOG_CONFIG_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../../configuration/default/configuration.h"
#include "../../configuration/default/default_pins.h"
#include "../../configuration/base/configbase.h"

class Serial_log_config : public Configbase{
public:
  Serial_log_config(byte id) : Configbase(id, Configurations::SERIAL_LOG_CONFIG) {
  }

  void getModuleConfiguration(JsonObject &root)
  {
    root["logLevel"] = (int)logLevel;
  }

  void setModuleConfiguration(JsonObject &root)
  {
    logLevel = static_cast<LogLevel>(int(root["logLevel"]));
  }

  LogLevel logLevel = LogLevel::DEBUG;
};

#endif
