#ifndef WHEEL_CONFIG_H
#define WHEEL_CONFIG_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include "./default/configuration.h"
#include "./default/default_pins.h"
#include "./base/configbase.h"
#include "../enums/modules.h"

class Wheel_config : public Configbase{
public:
  Wheel_config(byte id) : Configbase(id, Configurations::WHEEL_CONFIG) {
  }

  void getConfiguration(JsonObject &root)
  {
    root["id"] = id;
    root["configuration"] = (int)configuration;
    root["index"] = index;
    root["isElectric"] = isElectric;
    root["canSteer"] = canSteer;
    root["isInverse"] = isInverse;
  }

  void setConfiguration(JsonObject &root)
  {
    id = byte(root["id"]);
    index = byte(root["index"]);
    isElectric = bool(root["isElectric"]);
    canSteer = bool(root["canSteer"]);
    isInverse = bool(root["isInverse"]);
  }

  byte index = 0;
  bool isElectric = false;
  bool canSteer = false;
  bool isInverse = false;
};

#endif
