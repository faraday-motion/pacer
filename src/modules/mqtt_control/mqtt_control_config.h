#ifndef MQTT_CONTROL_CONFIG_H
#define MQTT_CONTROL_CONFIG_H

#include <memory>
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../../configuration/default/configuration.h"
#include "../../configuration/default/default_pins.h"
#include "../../configuration/base/configbase.h"

class Mqtt_control_config : public Configbase{
public:
  Mqtt_control_config(byte id) : Configbase(id, Configurations::MQTT_CONTROL_CONFIG) {
  }

  void getModuleConfiguration(JsonObject &root)
  {
    root["resetInputTimeout"] = resetInputTimeout;
  }

  void setModuleConfiguration(JsonObject &root)
  {
    resetInputTimeout = int(root["resetInputTimeout"]);
  }

  int resetInputTimeout = 10000;
};

#endif
