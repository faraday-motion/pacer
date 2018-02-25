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
  Current_monitor_config(byte id) : Configbase(id, Configurations::DEAD_MAN_SWITCH_CONFIG) {
  }

  void getModuleConfiguration(JsonObject &root)
  {
    root["sensorId"] = sensorId;
    root["minCurrent"] = minCurrent;
    root["maxCurrent"] = maxCurrent;
    root["sensorName"] = sensorName;
  }

  void setModuleConfiguration(JsonObject &root)
  {
    sensorId = byte(root["sensorId"]);
    minCurrent = int(root["minCurrent"]);
    maxCurrent = int(root["maxCurrent"]);
    sensorName = root["sensorName"].as<String>();
  }

  byte sensorId = 0;
  int minCurrent = 0;
  int maxCurrent = 1023;
  String sensorName = "";
};

#endif
