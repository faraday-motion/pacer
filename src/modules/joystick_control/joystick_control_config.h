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
    root["deadbandX"] = deadbandX;
    root["neutralX"] = neutralX;
    root["limitXMin"] = limitXMin;
    root["limitXMax"] = limitXMax;
    root["invertX"] = invertX;
    root["sensorNameX"] = sensorNameX;
    root["deadbandY"] = deadbandY;
    root["neutralY"] = neutralY;
    root["limitYMin"] = limitYMin;
    root["limitYMax"] = limitYMax;
    root["invertY"] = invertY;
    root["sensorNameY"] = sensorNameY;
    root["sensorEnabled"] = sensorEnabled;
  }

  void setModuleConfiguration(JsonObject &root)
  {
    deadbandX = int(root["deadbandX"]);
    neutralX = int(root["neutralX"]);
    limitXMin = int(root["limitXMin"]);
    limitXMax = int(root["limitXMax"]);
    invertX = bool(root["invertX"]);
    sensorNameX = root["sensorNameX"].as<String>();
    deadbandY = int(root["deadbandY"]);
    neutralY = int(root["neutralY"]);
    limitYMin = int(root["limitYMin"]);
    limitYMax = int(root["limitYMax"]);
    invertY = bool(root["invertY"]);
    sensorNameY = root["sensorNameY"].as<String>();
    sensorEnabled = root["sensorEnabled"].as<String>();
  }

  int deadbandX = 50;
  int neutralX = 512;
  int limitXMin = 0;
  int limitXMax = 1023;
  bool invertX = false;
  String sensorNameX = "";
  int deadbandY = 50;
  int neutralY = 512;
  int limitYMin = 50;
  int limitYMax = 1023;
  bool invertY = false;
  String sensorNameY = "";
  String sensorEnabled = "";
};

#endif
