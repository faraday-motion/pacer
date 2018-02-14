#ifndef WEBSOCKET_DRIVELOG_CONFIG_H
#define WEBSOCKET_DRIVELOG_CONFIG_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../../configuration/default/configuration.h"
#include "../../configuration/default/default_pins.h"
#include "../../configuration/base/configbase.h"
#include "../../enums/enums.hpp"

class Websocket_drivelog_config : public Configbase{
public:
  Websocket_drivelog_config(byte id) : Configbase(id, Configurations::WEBSOCKET_DRIVELOG_CONFIG) {
  }

  void getConfiguration(JsonObject &root)
  {
    root["id"] = id;
    root["configuration"] = (int)configuration;
    root["interval"] = (int)interval;
    root["critical"] = (int)critical;
    root["enabled"] = enabled;
  }

  void setConfiguration(JsonObject &root)
  {
    id = byte(root["id"]);
    interval = int(root["interval"]);
    critical = int(root["critical"]);
    enabled = bool(root["enabled"]);
  }

  int interval = 500;
  int critical = 1000;
  bool enabled = MODULE_DEFAULT_ENABLED;
};

#endif
