#ifndef FORCE_CONTROL_CONFIG_H
#define FORCE_CONTROL_CONFIG_H
#include <memory>
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../../configuration/default/configuration.h"
#include "../../configuration/default/default_pins.h"
#include "../../configuration/base/configbase.h"
#include "../../enums/enums.hpp"

class Force_control_config : public Configbase{
public:
  Force_control_config(byte id) : Configbase(id, Configurations::FORCE_CONTROL_CONFIG) {
  }

  void getConfiguration(JsonObject &root)
  {
    root["id"] = id;
    root["configuration"] = (int)configuration;
    root["sensorId"] = sensorId;
    root["deadbandY"] = deadbandY;
    root["neutralY"] = neutralY;
    root["limitYMin"] = limitYMin;
    root["limitYMax"] = limitYMax;
    root["invertY"] = invertY;
    root["enabled"] = enabled;
  }

  void setConfiguration(JsonObject &root)
  {
    id = byte(root["id"]);
    sensorId = byte(root["sensorId"]);
    deadbandY = int(root["deadbandY"]);
    neutralY = int(root["neutralY"]);
    limitYMin = int(root["limitYMin"]);
    limitYMax = int(root["limitYMax"]);
    invertY = bool(root["invertY"]);
    enabled = bool(root["enabled"]);
  }

  byte sensorId = 0;
  int deadbandY = 50;
  int neutralY = 512;
  int limitYMin = 0;
  int limitYMax = 1023;
  bool invertY = false;
  bool enabled = MODULE_DEFAULT_ENABLED;
};

#endif
