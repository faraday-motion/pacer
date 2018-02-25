#ifndef WIFI_SIMPLE_CONTROL_CONFIG_H
#define WIFI_SIMPLE_CONTROL_CONFIG_H
#include <memory>
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../../configuration/default/configuration.h"
#include "../../configuration/default/default_pins.h"
#include "../../configuration/base/configbase.h"
#include "../../enums/enums.hpp"

class Wifi_simple_control_config : public Configbase{
public:
  Wifi_simple_control_config(byte id) : Configbase(id, Configurations::WIFI_SIMPLE_CONTROL_CONFIG) {
  }

  void getModuleConfiguration(JsonObject &root)
  {
    root["inputTimeout"] = inputTimeout;
  }

  void setModuleConfiguration(JsonObject &root)
  {
    inputTimeout = int(root["inputTimeout"]);
  }

  int inputTimeout = 500;
};

#endif
