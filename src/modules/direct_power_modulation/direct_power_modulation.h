/*
#ifndef DIRECT_POWER_MODULATION_H
#define DIRECT_POWER_MODULATION_H

#include <Arduino.h>
#include "../base/base.hpp"
#include "../../configuration/configurator.h"
#include "./direct_power_modulation_config.h"
#include "../../utility/simpletimer.h"
#include "../../interfaces/interfaces.hpp"

class Direct_power_modulation : public Modulation_module
{
private:
  IFMV * mFMV;
  SimpleTimer mSimpleTimer;
protected:
    void onDisable();
public:
  Direct_power_modulation(byte id, IFMV * fmv) : Modulation_module(id, Modules::DIRECT_POWER_MODULATION)  {
    mFMV = fmv;
    setConfig();
  }

  void setConfig()
  {
    mSimpleTimer.setInterval(25, 50);
  }

  enum Events : byte {
    CONFIGURE,
  };

  void setup();
  void loop();
  void command(byte command);

  String getModuleName()
  {
    return FPSTR("DIRECT_POWER_MODULATION");
  }
};

#endif
*/
