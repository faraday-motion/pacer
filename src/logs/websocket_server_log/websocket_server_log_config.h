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

  void getConfiguration(JsonObject &root)
  {
    root["id"] = id;
    root["configuration"] = (int)configuration;
    root["port"] = port;
    root["logLevel"] = (int)logLevel;
    root["enabled"] = enabled;
  }

  void setConfiguration(JsonObject &root)
  {
    id = byte(root["id"]);
    port = int(root["port"]);
    logLevel = static_cast<LogLevel>(int(root["logLevel"]));
    enabled = bool(root["enabled"]);
  }

  int port = WEBSOCKET_PORT;
  LogLevel logLevel = LogLevel::DEBUG;
  bool enabled = LOGGER_LOG_DEFALT_ENABLED;
};

#endif
