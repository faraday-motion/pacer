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
    root["hostName"] = hostName;
    root["password"] = password;
  }

  void setConfiguration(JsonObject &root)
  {
    id = byte(root["id"]);
    hostName = root["hostName"].as<String>();
    password = root["password"].as<String>();
  }

  int port = ARDUINO_OTA_PORT;
  String hostName = ARDUINO_OTA_HOSTNAME;
  String password = ARDUINO_OTA_PASSWORD;
};

#endif
