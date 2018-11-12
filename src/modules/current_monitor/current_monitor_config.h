#ifndef CURRENT_MONITOR_CONFIG_H
#define CURRENT_MONITOR_CONFIG_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../../configuration/default/configuration.h"
#include "../../configuration/default/default_pins.h"
#include "../../configuration/base/configbase.h"
#include "../../enums/enums.hpp"

class Current_monitor_config : public Configbase{
public:
  Current_monitor_config(byte id) : Configbase(id, Configurations::CURRENT_MONITOR_CONFIG) {
  }

  void getModuleConfiguration(JsonObject &root)
  {
    root["minCurrent"] = minCurrent;
    root["maxCurrent"] = maxCurrent;
    root["sensorName"] = sensorName;
  }

  void setModuleConfiguration(JsonObject &root)
  {
    minCurrent = float(root["minCurrent"]);
    maxCurrent = float(root["maxCurrent"]);
    sensorName = root["sensorName"].as<String>();
  }

  float minCurrent = 0;
  float maxCurrent = 1023;
  String sensorName = "";
};

#endif
