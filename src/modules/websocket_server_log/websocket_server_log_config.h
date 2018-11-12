#ifndef WEBSOCKET_SERVER_LOG_CONFIG_H
#define WEBSOCKET_SERVER_LOG_CONFIG_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../../configuration/default/configuration.h"
#include "../../configuration/default/default_pins.h"
#include "../../configuration/base/configbase.h"
#include "../../enums/enums.hpp"

class Websocket_server_log_config : public Configbase{
public:
  Websocket_server_log_config(byte id) : Configbase(id, Configurations::WEBSOCKET_SERVER_LOG_CONFIG) {
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
