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
    root["smoothAlphaPositive"] = smoothAlphaPositive;
    root["smoothAlphaNegative"] = smoothAlphaNegative;
    root["interval"] = interval;
    root["critical"] = critical;
  }

  void setModuleConfiguration(JsonObject &root)
  {
    smoothAlphaPositive = float(root["smoothAlphaPositive"]);
    smoothAlphaNegative = float(root["smoothAlphaNegative"]);
    interval = int(root["interval"]);
    critical = int(root["critical"]);
  }

  float smoothAlphaPositive = SMOOTH_ALPHA_POSITIVE;
  float smoothAlphaNegative = SMOOTH_ALPHA_NEGATIVE;
  int interval = 50;
  int critical = 60;
};

#endif
