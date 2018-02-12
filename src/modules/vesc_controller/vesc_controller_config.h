#ifndef VESC_CONTROLLER_CONFIG_H
#define VESC_CONTROLLER_CONFIG_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../../configuration/default/configuration.h"
#include "../../configuration/default/default_pins.h"
#include "../../configuration/base/configbase.h"
#include "../../enums/enums.hpp"

class Vesc_controller_config : public Configbase{
public:
  Vesc_controller_config(byte id) : Configbase(id, Configurations::VESC_CONTROLLER_CONFIG) {
  }

  void getConfiguration(JsonObject &root)
  {
    root["id"] = id;
    root["configuration"] = (int)configuration;
    root["maxPowerCurrent"] = maxPowerCurrent;
    root["maxBrakeCurrent"] = maxBrakeCurrent;
    root["maxRpm"] = maxRpm;
    root["defaultSerial"] = defaultSerial;
    root["enabled"] = enabled;
  }

  void setConfiguration(JsonObject &root)
  {
    id = byte(root["id"]);
    maxPowerCurrent = byte(root["maxPowerCurrent"]);
    maxBrakeCurrent = byte(root["maxBrakeCurrent"]);
    maxRpm = int(root["maxRpm"]);
    defaultSerial = root["defaultSerial"];
    enabled = root["enabled"];
  }

  byte maxPowerCurrent = 60;
  byte maxBrakeCurrent = 60;
  int maxRpm = 0;
  bool defaultSerial = true;
  bool enabled = MODULE_DEFAULT_ENABLED;
};

#endif
