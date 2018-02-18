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

  void getConfiguration(JsonObject &root)
  {
    root["id"] = id;
    root["configuration"] = (int)configuration;
    root["logLevel"] = (int)logLevel;
    root["enabled"] = enabled;
  }

  void setConfiguration(JsonObject &root)
  {
    id = byte(root["id"]);
    logLevel = static_cast<LogLevel>(int(root["logLevel"]));
    enabled = bool(root["enabled"]);
  }

  LogLevel logLevel = LogLevel::DEBUG;
  bool enabled = LOGGER_LOG_DEFALT_ENABLED;
};

#endif