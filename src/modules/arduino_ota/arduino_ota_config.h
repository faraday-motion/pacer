#ifndef ARDUINO_OTA_CONFIG_H
#define ARDUINO_OTA_CONFIG_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../../configuration/default/configuration.h"
#include "../../configuration/default/default_pins.h"
#include "../../configuration/base/configbase.h"
#include "../../enums/enums.hpp"

class Arduino_ota_config : public Configbase{
public:
  Arduino_ota_config(byte id) : Configbase(id, Configurations::ARDUINO_OTA_CONFIG) {
  }

  void getConfiguration(JsonObject &root)
  {
    root["id"] = id;
    root["configuration"] = (int)configuration;
  }

  void setConfiguration(JsonObject &root)
  {
    id = byte(root["id"]);
  }

};

#endif
