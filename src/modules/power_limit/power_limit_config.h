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

  void getModuleConfiguration(JsonObject &root)
  {
    root["deadSensorName"] = deadSensorName;
  }

  void setModuleConfiguration(JsonObject &root)
  {
    deadSensorName = root["deadSensorName"].as<String>();
  }

  String deadSensorName = "";
};

#endif
