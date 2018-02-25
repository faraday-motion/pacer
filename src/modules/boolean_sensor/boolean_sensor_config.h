#ifndef BOOLEAN_SENSOR_CONFIG_H
#define BOOLEAN_SENSOR_CONFIG_H
#include <memory>
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../../configuration/default/configuration.h"
#include "../../configuration//base/configbase.h"
#include "../../enums/enums.hpp"

class Boolean_sensor_config : public Configbase{
public:
  Boolean_sensor_config(byte id) : Configbase(id, Configurations::BOOLEAN_SENSOR_CONFIG) {
  }

  void getModuleConfiguration(JsonObject &root)
  {
    root["pin"] = pin;
    root["interval"] = interval;
    root["critical"] = critical;
    root["invert"] = invert;
    root["sensorName"] = sensorName;
  }

  void setModuleConfiguration(JsonObject &root)
  {
    pin = byte(root["pin"]);
    interval = int(root["interval"]);
    critical = int(root["critical"]);
    invert = bool(root["invert"]);
    sensorName = root["sensorName"].as<String>();;
  }

  byte pin = 0;
  int interval = 500;
  int critical = 1000;
  bool invert = false;
  String sensorName = "";
};

#endif
