#ifndef SWITCH_CONFIG_H
#define SWITCH_CONFIG_H
#include <memory>
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../../configuration/default/configuration.h"
#include "../../configuration//base/configbase.h"
#include "../../enums/enums.hpp"

class Switch_config : public Configbase{
public:
  Switch_config(byte id) : Configbase(id, Configurations::SWITCH_CONFIG) {
  }

  void getConfiguration(JsonObject &root)
  {
    root["id"] = id;
    root["configuration"] = (int)configuration;
    root["pin"] = pin;
    root["interval"] = interval;
    root["critical"] = critical;
    root["invert"] = invert;
  }

  void setConfiguration(JsonObject &root)
  {
    id = byte(root["id"]);
    pin = byte(root["pin"]);
    interval = int(root["interval"]);
    critical = int(root["critical"]);
    invert = bool(root["invert"]);
  }

  byte pin = 0;
  int interval = 500;
  int critical = 1000;
  bool invert = false;
};

#endif
