#ifndef DIRECT_POWER_MODULATION_CONFIG_H
#define DIRECT_POWER_MODULATION_CONFIG_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../../configuration/default/configuration.h"
#include "../../configuration/default/default_pins.h"
#include "../../configuration/base/configbase.h"


class Direct_power_modulation_config : public Configbase{
public:
  Direct_power_modulation_config(byte id) : Configbase(id, Configurations::DIRECT_POWER_MODULATION_CONFIG) {

  }

  void getConfiguration(JsonObject &root)
  {
    root["id"] = id;
    root["configuration"] = (int)configuration;
    root["interval"] = interval;
    root["critical"] = critical;
  }

  void setConfiguration(JsonObject &root)
  {
    id = byte(root["id"]);
    interval = int(root["interval"]);
    critical = int(root["critical"]);
  }

  int interval = 15;
  int critical = 30;
};

#endif
