#ifndef WEBSOCKET_CONNECTION_CONFIG_H
#define WEBSOCKET_CONNECTION_CONFIG_H

#include <memory>
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../../configuration/default/configuration.h"
#include "../../configuration/default/default_pins.h"
#include "../../configuration/base/configbase.h"

class Websocket_connection_config : public Configbase{
public:
  Websocket_connection_config(byte id) : Configbase(id, Configurations::WEBSOCKET_CONNECTION_CONFIG) {
  }

  void getConfiguration(JsonObject &root)
  {
    root["id"] = id;
    root["configuration"] = (int)configuration;
    root["enabled"] = enabled;
    root["port"] = port;
    root["clientMax"] = clientMax;
  }

  void setConfiguration(JsonObject &root)
  {
    id = byte(root["id"]);
    enabled = bool(root["enabled"]);
    port = int(root["port"]);
    clientMax = byte(root["clientMax"]);
  }

  bool enabled = MODULE_DEFAULT_ENABLED;
  int port = WEBSOCKET_PORT;
  byte clientMax = 2;
};

#endif
