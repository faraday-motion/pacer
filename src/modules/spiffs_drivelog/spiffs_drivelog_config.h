#ifndef SPIFFS_DRIVELOG_CONFIG_H
#define SPIFFS_DRIVELOG_CONFIG_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../../configuration/default/configuration.h"
#include "../../configuration/default/default_pins.h"
#include "../../configuration/base/configbase.h"
#include "../../enums/enums.hpp"

class Spiffs_drivelog_config : public Configbase{
public:
  Spiffs_drivelog_config(byte id) : Configbase(id, Configurations::SPIFFS_DRIVELOG_CONFIG) {
  }

  void getConfiguration(JsonObject &root)
  {
    root["id"] = id;
    root["configuration"] = (int)configuration;
    root["interval"] = (int)interval;
    root["critical"] = (int)critical;
    root["driveLog"] = driveLog;
    root["enabled"] = enabled;
  }

  void setConfiguration(JsonObject &root)
  {
    id = byte(root["id"]);
    interval = int(root["interval"]);
    critical = int(root["critical"]);
    driveLog = root["driveLog"].as<String>();
    enabled = bool(root["enabled"]);
  }

  int interval = 500;
  int critical = 1000;
  String driveLog = SPIFF_DRIVELOG_FILENAME;
  bool enabled = MODULE_DEFAULT_ENABLED;
};

#endif
