#ifndef CONFIGBASE_H
#define CONFIGBASE_H
#include <memory>
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../../enums/configurations.h"
#include "../../enums/modules.h"
#include "../../interfaces/interfaces.hpp"


class Configbase : public IConfigurable{
private:
protected:
  Configbase(byte id, Configurations configuration) : IConfigurable() {
    this -> id = id;
    this -> configuration = configuration;
  }
public:
  byte id = 0;
  Configurations configuration = Configurations::NONE;
  virtual void getConfiguration(JsonObject &root);
  virtual void setConfiguration(JsonObject &root);
};

#endif
