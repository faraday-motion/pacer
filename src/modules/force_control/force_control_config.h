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

  void getModuleConfiguration(JsonObject &root)
  {
    root["deadbandY"] = deadbandY;
    root["neutralY"] = neutralY;
    root["limitYMin"] = limitYMin;
    root["limitYMax"] = limitYMax;
    root["invertY"] = invertY;
    root["sensorName"] = sensorName;
  }

  void setModuleConfiguration(JsonObject &root)
  {
    deadbandY = int(root["deadbandY"]);
    neutralY = int(root["neutralY"]);
    limitYMin = int(root["limitYMin"]);
    limitYMax = int(root["limitYMax"]);
    invertY = bool(root["invertY"]);
    sensorName = root["sensorName"].as<String>();
  }

  int deadbandY = 50;
  int neutralY = 512;
  int limitYMin = 0;
  int limitYMax = 1023;
  bool invertY = false;
  String sensorName = "";
};

#endif
