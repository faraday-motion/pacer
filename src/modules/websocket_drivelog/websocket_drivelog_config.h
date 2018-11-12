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

  void getModuleConfiguration(JsonObject &root)
  {
    root["interval"] = (int)interval;
    root["critical"] = (int)critical;
  }

  void setModuleConfiguration(JsonObject &root)
  {
    interval = int(root["interval"]);
    critical = int(root["critical"]);
  }

  int interval = 500;
  int critical = 1000;
};

#endif
