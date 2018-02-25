#ifndef ANALOG_SENSOR_CONFIG_H
#define ANALOG_SENSOR_CONFIG_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../../configuration/default/configuration.h"
#include "../../configuration/base/configbase.h"
#include "../../enums/enums.hpp"

class Analog_sensor_config : public Configbase{
public:
  Analog_sensor_config(byte id) : Configbase(id, Configurations::ANALOG_SENSOR_CONFIG) {
  }

  void getModuleConfiguration(JsonObject &root)
  {
    root["pin"] = pin;
    root["interval"] = interval;
    root["critical"] = critical;
    root["sensorName"] = sensorName;
  }

  void setModuleConfiguration(JsonObject &root)
  {
    pin = byte(root["pin"]);
    interval = int(root["interval"]);
    critical = int(root["critical"]);
    sensorName = root["sensorName"].as<String>();
  }

  byte pin = 0;
  int interval = 25;
  int critical = 50;
  String sensorName = "";
};

#endif
