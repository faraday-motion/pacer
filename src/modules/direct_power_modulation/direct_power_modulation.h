#ifndef DIRECT_POWER_MODULATION_H
#define DIRECT_POWER_MODULATION_H
/*
#include <Arduino.h>
#include "../../configuration/configurator.h"
#include "./direct_power_modulation_config.h"
#include "../../utility/simpletimer.h"
#include "../base/input_modulation.h"
#include "../../fmv.h"

class Direct_power_modulation : virtual public Input_modulation
{
private:
  FMV *mFMV;
  SimpleTimer mSimpleTimer;
protected:
    void onDisable();
public:
  Direct_power_modulation(byte id, FMV *fmv) : Input_modulation(id, Modules::DIRECT_POWER_MODULATION)  {
    mFMV = fmv;
    setConfig();
  }

  void setConfig()
  {
    mSimpleTimer.setInterval(25, 50);
  }

  void setup();
  void loop();
  void command(byte command);
};
*/
#endif
