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

  void getModuleConfiguration(JsonObject &root)
  {
    root["interval"] = (int)interval;
    root["critical"] = (int)critical;
    root["driveLog"] = driveLog;
  }

  void setModuleConfiguration(JsonObject &root)
  {
    interval = int(root["interval"]);
    critical = int(root["critical"]);
    driveLog = root["driveLog"].as<String>();
  }

  int interval = 500;
  int critical = 1000;
  String driveLog = SPIFF_DRIVELOG_FILENAME;
};

#endif
