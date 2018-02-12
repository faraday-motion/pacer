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

  void getConfiguration(JsonObject &root)
  {
    root["id"] = id;
    root["configuration"] = (int)configuration;
    root["enabled"] = enabled;
    root["inputTimeout"] = inputTimeout;
  }

  void setConfiguration(JsonObject &root)
  {
    id = byte(root["id"]);
    enabled = bool(root["enabled"]);
    inputTimeout = int(root["inputTimeout"]);
  }

  byte sensorId = 0;
  bool enabled = MODULE_DEFAULT_ENABLED;
  int inputTimeout = 500;
};

#endif
