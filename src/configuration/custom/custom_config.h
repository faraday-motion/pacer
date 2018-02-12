#ifndef CUSTOM_CONFIG_H
#define CUSTOM_CONFIG_H

#include <Arduino.h>
#include "../default/default_config.h"
#include "../default/configuration.h"
#include "../configurator.h"
class Custom_config : public Default_config{
public:
  Custom_config() : Default_config() {

  }

  /*
  void addConfigs()
  {

  }
  */
};

#endif
