#ifndef WEB_UPDATE_CONFIG_H
#define WEB_UPDATE_CONFIG_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../../configuration/default/configuration.h"
#include "../../configuration/default/default_pins.h"
#include "../../configuration/base/configbase.h"
#include "../../enums/enums.hpp"

class Web_update_config : public Configbase{
public:
  Web_update_config(byte id) : Configbase(id, Configurations::WEB_UPDATE_CONFIG) {
  }

  void getModuleConfiguration(JsonObject &root)
  {
  }

  void setModuleConfiguration(JsonObject &root)
  {
  }

};

#endif
