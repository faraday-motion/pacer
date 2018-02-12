#ifndef POWER_LIMIT_CONFIG_H
#define POWER_LIMIT_CONFIG_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../../configuration/default/configuration.h"
#include "../../configuration/default/default_pins.h"
#include "../../configuration/base/configbase.h"
#include "../../enums/enums.hpp"

class Power_limit_config : public Configbase{
public:
  Power_limit_config(byte id) : Configbase(id, Configurations::POWER_LIMIT_CONFIG) {
  }

  void getConfiguration(JsonObject &root)
  {
    root["id"] = id;
    root["configuration"] = (int)configuration;
  }

  void setConfiguration(JsonObject &root)
  {
    id = byte(root["id"]);
  }

};

#endif
