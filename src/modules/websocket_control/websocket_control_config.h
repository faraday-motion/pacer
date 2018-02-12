#ifndef WEBSOCKET_CONTROL_CONFIG_H
#define WEBSOCKET_CONTROL_CONFIG_H

#include <memory>
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../../configuration/default/configuration.h"
#include "../../configuration/default/default_pins.h"
#include "../../configuration/base/configbase.h"

class Websocket_control_config : public Configbase{
public:
  Websocket_control_config(byte id) : Configbase(id, Configurations::WEBSOCKET_CONTROL_CONFIG) {
  }

  void getConfiguration(JsonObject &root)
  {
    root["id"] = id;
    root["configuration"] = (int)configuration;
    root["enabled"] = enabled;
    root["resetInputTimeout"] = resetInputTimeout;
    root["port"] = port;
    root["websocketServerLogModuleId"] = websocketServerLogModuleId;
    root["clientMax"] = clientMax;
  }

  void setConfiguration(JsonObject &root)
  {
    id = byte(root["id"]);
    enabled = bool(root["enabled"]);
    resetInputTimeout = int(root["resetInputTimeout"]);
    port = int(root["port"]);
    websocketServerLogModuleId = int(root["websocketServerLogModuleId"]);
    clientMax = byte(root["clientMax"]);
  }

  bool enabled = MODULE_DEFAULT_ENABLED;
  int resetInputTimeout = 10000;
  int port = WEBSOCKET_PORT;
  int websocketServerLogModuleId = -1;
  byte clientMax = 2;
};

#endif
