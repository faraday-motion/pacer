#ifndef DEAD_MAN_SWITCH_CONFIG_H
#define DEAD_MAN_SWITCH_CONFIG_H
#include <memory>
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../../configuration/default/configuration.h"
#include "../../configuration/default/default_pins.h"
#include "../../configuration/base/configbase.h"
#include "../../enums/enums.hpp"

class Dead_man_switch_config : public Configbase{
public:
  Dead_man_switch_config(byte id) : Configbase(id, Configurations::DEAD_MAN_SWITCH_CONFIG) {
  }

  void getConfiguration(JsonObject &root)
  {
    root["id"] = id;
    root["configuration"] = (int)configuration;
    root["sensorId"] = sensorId;
  }

  void setConfiguration(JsonObject &root)
  {
    id = byte(root["id"]);
    sensorId = byte(root["sensorId"]);
  }

  byte sensorId = 0;
};

#endif
