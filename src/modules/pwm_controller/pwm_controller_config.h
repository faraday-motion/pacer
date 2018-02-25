#ifndef PWM_CONTROLLER_CONFIG_H
#define PWM_CONTROLLER_CONFIG_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../../configuration/default/configuration.h"
#include "../../configuration/default/default_pins.h"
#include "../../configuration/base/configbase.h"
#include "../../enums/enums.hpp"

class Pwm_controller_config : public Configbase{
public:
  Pwm_controller_config(byte id) : Configbase(id, Configurations::PWM_CONTROLLER_CONFIG) {
  }

  void getModuleConfiguration(JsonObject &root)
  {
    JsonArray& pins = root.createNestedArray("servoPins");
    pins.add(servoPins[0]);
    pins.add(servoPins[1]);
    pins.add(servoPins[2]);
    pins.add(servoPins[3]);
    root["minAngle"] = minAngle;
    root["maxAngle"] = maxAngle;
    root["neutral"] = neutral;
  }

  void setModuleConfiguration(JsonObject &root)
  {
    servoPins[0] = root["servoPins"][0];
    servoPins[1] = root["servoPins"][1];
    servoPins[2] = root["servoPins"][2];
    servoPins[3] = root["servoPins"][3];
    minAngle = byte(root["minAngle"]);
    maxAngle = byte(root["maxAngle"]);
    neutral = byte(root["neutral"]);
  }

  byte servoPins[4] = {27, 26, 12, 14};
  byte minAngle = 0;
  byte maxAngle = 180;
  byte neutral = 90;
};

#endif
