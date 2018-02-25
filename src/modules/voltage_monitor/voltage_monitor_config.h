#ifndef VOLTAGE_MONITOR_CONFIG_H
#define VOLTAGE_MONITOR_CONFIG_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../../configuration/default/configuration.h"
#include "../../configuration/default/default_pins.h"
#include "../../configuration/base/configbase.h"
#include "../../enums/enums.hpp"

class Voltage_monitor_config : public Configbase{
public:
  Voltage_monitor_config(byte id) : Configbase(id, Configurations::VOLTAGE_MONITOR_CONFIG) {
  }

  void getModuleConfiguration(JsonObject &root)
  {
    root["sensorId"] = sensorId;
    root["minVoltage"] = minVoltage;
    root["maxVoltage"] = maxVoltage;
    root["sensorName"] = sensorName;
  }

  void setModuleConfiguration(JsonObject &root)
  {
    sensorId = byte(root["sensorId"]);
    minVoltage = byte(root["minVoltage"]);
    maxVoltage = int(root["maxVoltage"]);
    sensorName = root["sensorName"].as<String>();
  }

  byte sensorId = 0;
  int minVoltage = 0;
  int maxVoltage = 1023;
  String sensorName = "";
};

#endif
