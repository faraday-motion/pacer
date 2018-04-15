#ifndef CUSTOM_SAFE_CONFIG_H
#define CUSTOM_SAFE_CONFIG_H

#include <Arduino.h>
#include "../default/default_safe_config.h"
#include "../default/configuration.h"
#include "../configurator.h"
class Custom_safe_config : public Default_safe_config{
public:
  Custom_safe_config() : Default_safe_config() {

  }

  /*
  void addConfigs()
  {

  }
  */

};

#endif
