#ifndef ANALOG_CONFIG_H
#define ANALOG_CONFIG_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../../configuration/default/configuration.h"
#include "../../configuration/base/configbase.h"
#include "../../enums/enums.hpp"

class Analog_config : public Configbase{
public:
  Analog_config(byte id) : Configbase(id, Configurations::ANALOG_CONFIG) {
  }

  void getConfiguration(JsonObject &root)
  {
    root["id"] = id;
    root["configuration"] = (int)configuration;
    root["pin"] = pin;
    root["interval"] = interval;
    root["critical"] = critical;
  }

  void setConfiguration(JsonObject &root)
  {
    id = byte(root["id"]);
    configuration = static_cast<Configurations>(int(root["configuration"]));
    pin = byte(root["pin"]);
    interval = int(root["interval"]);
    critical = int(root["critical"]);
  }

  byte pin = 0;
  int interval = 25;
  int critical = 50;
};

#endif
