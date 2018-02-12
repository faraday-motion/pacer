#ifndef SERIAL_CONTROL_CONFIG_H
#define SERIAL_CONTROL_CONFIG_H
#include <memory>
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../../configuration/default/configuration.h"
#include "../../configuration/default/default_pins.h"
#include "../../configuration/base/configbase.h"
#include "../../enums/enums.hpp"

class Serial_control_config : public Configbase{
public:
  Serial_control_config(byte id) : Configbase(id, Configurations::SERIAL_CONTROL_CONFIG) {
  }

  void getConfiguration(JsonObject &root)
  {
    root["id"] = id;
    root["configuration"] = (int)configuration;
    root["enabled"] = enabled;
    root["resetInputTimeout"] = resetInputTimeout;
  }

  void setConfiguration(JsonObject &root)
  {
    id = byte(root["id"]);
    enabled = bool(root["enabled"]);
    resetInputTimeout = int(root["resetInputTimeout"]);
  }

  bool enabled = MODULE_DEFAULT_ENABLED;
  int resetInputTimeout = 10000;
};

#endif
