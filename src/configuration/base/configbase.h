#ifndef CONFIGBASE_H
#define CONFIGBASE_H
#include <memory>
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../../configuration/default/configuration.h"
#include "../../enums/configurations.h"
#include "../../enums/modules.h"
#include "../../interfaces/interfaces.hpp"

class Configbase : public IConfig{
private:
  byte id = 0;
protected:
  Configbase(byte id, int configuration) : IConfig() {
    this -> id = id;
    this -> configuration = configuration;
  }
  virtual void getModuleConfiguration(JsonObject &root);
  virtual void setModuleConfiguration(JsonObject &root);
public:
  int configuration = Configurations::NONE;
  bool enabled = MODULE_DEFAULT_ENABLED;

  byte getId()
  {
    return id;
  }

  void setId(byte id)
  {
    this -> id = id;
  }

  bool getEnabled()
  {
    return enabled;
  }

  void getConfiguration(JsonObject &root)
  {
    root["id"] = id;
    root["configuration"] = (int)configuration;
    root["enabled"] = enabled;
    getModuleConfiguration(root);
  }

  void setConfiguration(JsonObject &root)
  {
    id = byte(root["id"]);
    configuration = int(root["configuration"]);
    enabled = bool(root["enabled"]);
    setModuleConfiguration(root);
  }
};

#endif
