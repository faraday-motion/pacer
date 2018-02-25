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

  void getModuleConfiguration(JsonObject &root)
  {
    root["logFile"] = logFile;
    root["logLevel"] = (int)logLevel;
  }

  void setModuleConfiguration(JsonObject &root)
  {
    logLevel = static_cast<LogLevel>(int(root["logLevel"]));
    logFile = root["logFile"].as<String>();
  }

  String logFile = "/logfile.txt";
  LogLevel logLevel = LogLevel::DEBUG;
};

#endif
