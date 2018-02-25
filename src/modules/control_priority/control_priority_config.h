#ifndef CONTROL_PRIORITY_CONGIG_H
#define CONTROL_PRIORITY_CONGIG_H
#include <memory>
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../../configuration/default/configuration.h"
#include "../../configuration/default/default_pins.h"
#include "../../configuration/base/configbase.h"

class Control_priority_config : public Configbase{
public:
  Control_priority_config(byte id) : Configbase(id, Configurations::CONTROL_PRIORITY_CONFIG) {
  }

  void getModuleConfiguration(JsonObject &root)
  {
    root["interval"] = interval;
    root["priority1"] = priority1;
    root["priority2"] = priority2;
    root["priority3"] = priority3;
    root["priority4"] = priority4;
    root["priority5"] = priority5;
  }

  void setModuleConfiguration(JsonObject &root)
  {
    interval = int(root["interval"]);
    priority1 = int(root["priority1"]);
    priority2 = int(root["priority2"]);
    priority3 = int(root["priority3"]);
    priority4 = int(root["priority4"]);
    priority5 = int(root["priority5"]);
  }

  int interval = 250;
  int priority1 = -1;
  int priority2 = -1;
  int priority3 = -1;
  int priority4 = -1;
  int priority5 = -1;
};

#endif
