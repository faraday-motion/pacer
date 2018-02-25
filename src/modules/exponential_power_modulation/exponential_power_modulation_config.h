#ifndef EXPONENTIAL_POWER_MODULATION_CONFIG_H
#define EXPONENTIAL_POWER_MODULATION_CONFIG_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../../configuration/default/configuration.h"
#include "../../configuration/default/default_pins.h"
#include "../../configuration/base/configbase.h"
#include "../../enums/enums.hpp"


class Exponential_power_modulation_config : public Configbase{
public:
  Exponential_power_modulation_config(byte id) : Configbase(id, Configurations::EXPONENTIAL_POWER_MODULATION_CONFIG) {

  }

  void getModuleConfiguration(JsonObject &root)
  {
    root["smoothAlpha"] = smoothAlpha;
    root["interval"] = interval;
    root["critical"] = critical;
  }

  void setModuleConfiguration(JsonObject &root)
  {
    smoothAlpha = float(root["smoothAlpha"]);
    interval = int(root["interval"]);
    critical = int(root["critical"]);
  }

  float smoothAlpha = DEFAULTSMOOTHALPHA;
  int interval = 15;
  int critical = 30;
};

#endif
