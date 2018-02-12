#ifndef TEMPERATURE_MONITOR_CONFIG_H
#define TEMPERATURE_MONITOR_CONFIG_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../../configuration/default/configuration.h"
#include "../../configuration/default/default_pins.h"
#include "../../configuration/base/configbase.h"
#include "../../enums/enums.hpp"

class Temperature_monitor_config : public Configbase{
public:
  Temperature_monitor_config(byte id) : Configbase(id, Configurations::TEMPERATURE_MONITOR_CONFIG) {
  }

  void getConfiguration(JsonObject &root)
  {
    root["id"] = id;
    root["configuration"] = (int)configuration;
    root["sensorId"] = sensorId;
    root["minTemperature"] = minTemperature;
    root["maxTemperature"] = maxTemperature;
  }

  void setConfiguration(JsonObject &root)
  {
    id = byte(root["id"]);
    sensorId = byte(root["sensorId"]);
    minTemperature = int(root["minTemperature"]);
    maxTemperature = int(root["maxTemperature"]);
  }

  byte sensorId = 0;
  int minTemperature = 0;
  int maxTemperature = 1023;
};

#endif
