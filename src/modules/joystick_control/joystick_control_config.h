#ifndef JOYSTICK_CONTROL_CONFIG_H
#define JOYSTICK_CONTROL_CONFIG_H
#include <memory>
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../../configuration/default/configuration.h"
#include "../../configuration/default/default_pins.h"
#include "../../configuration/base/configbase.h"

class Joystick_control_config : public Configbase{
public:
  Joystick_control_config(byte id) : Configbase(id, Configurations::JOYSTICK_CONTROL_CONFIG) {
  }

  void getModuleConfiguration(JsonObject &root)
  {
    root["sensorYId"] = sensorYId;
    root["deadbandY"] = deadbandY;
    root["neutralY"] = neutralY;
    root["limitYMin"] = limitYMin;
    root["limitYMax"] = limitYMax;
    root["invertY"] = invertY;
    root["sensorNameX"] = sensorNameX;
    root["sensorNameY"] = sensorNameY;
  }

  void setModuleConfiguration(JsonObject &root)
  {
    sensorYId = byte(root["sensorYId"]);
    deadbandY = int(root["deadbandY"]);
    neutralY = int(root["neutralY"]);
    limitYMin = int(root["limitYMin"]);
    limitYMax = int(root["limitYMax"]);
    invertY = bool(root["invertY"]);
    sensorNameX = root["sensorNameX"].as<String>();
    sensorNameY = root["sensorNameY"].as<String>();
  }

  byte sensorYId = 0;
  int deadbandY = 50;
  int neutralY = 512;
  int limitYMin = 0;
  int limitYMax = 1023;
  bool invertY = false;
  String sensorNameX = "";
  String sensorNameY = "";
};

#endif
