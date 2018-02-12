#ifndef SPIFFS_LOG_CONFIG_H
#define SPIFFS_LOG_CONFIG_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../../configuration/default/configuration.h"
#include "../../configuration/default/default_pins.h"
#include "../../configuration/base/configbase.h"
#include "../../enums/enums.hpp"

class Spiffs_log_config : public Configbase{
public:
  Spiffs_log_config(byte id) : Configbase(id, Configurations::SPIFFS_LOG_CONFIG) {
  }

  void getConfiguration(JsonObject &root)
  {
    root["id"] = id;
    root["configuration"] = (int)configuration;
    root["logFile"] = logFile;
    root["logLevel"] = (int)logLevel;
    root["enabled"] = enabled;
  }

  void setConfiguration(JsonObject &root)
  {
    id = byte(root["id"]);
    enabled = bool(root["enabled"]);
    logLevel = static_cast<LogLevel>(int(root["logLevel"]));
    logFile = root["logFile"].as<String>();
  }

  String logFile = "/logfile.txt";
  LogLevel logLevel = LogLevel::DEBUG;
  bool enabled = LOGGER_LOG_DEFALT_ENABLED;
};

#endif
