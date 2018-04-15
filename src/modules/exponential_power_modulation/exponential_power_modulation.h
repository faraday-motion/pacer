#ifndef EXPONENTIAL_POWER_MODULATION_H
#define EXPONENTIAL_POWER_MODULATION_H

#include <Arduino.h>
#include "../base/base.hpp"
//#include "../../configuration/configurator.h"
#include "./exponential_power_modulation_config.h"
#include "../../utility/simpletimer.h"
#include "../../interfaces/interfaces.hpp"

class Exponential_power_modulation : public Modulation_module
{
private:
  IFMV * mFMV = nullptr;
  SimpleTimer mSimpleTimer;
  byte mPreviousPower = 0;
  byte mPreviousBrake = 0;
  Exponential_power_modulation_config * mCfg = nullptr;
  byte calculateOutput(byte previousPower, byte targetPower, byte maxPower);
protected:
  void onDisable();
  void onEvent(byte eventId, bool always = false)
  {
    mFMV -> moduleEvent(this, eventId);
  }
public:
  Exponential_power_modulation(byte id, IFMV * fmv, Exponential_power_modulation_config * cfg = nullptr) : Modulation_module(id, Modules::EXPONENTIAL_POWER_MODULATION)  {
    mFMV = fmv;
    if (cfg == nullptr)
      mCfg = static_cast<Exponential_power_modulation_config*>(Configurator::Instance().getConfig(id, Configurations::EXPONENTIAL_POWER_MODULATION_CONFIG));
    else
      mCfg = cfg;
    setConfig();
  }

  enum Events : byte {
    CONFIGURE,
  };

  void setConfig()
  {
    mSimpleTimer.setName("Exponential_power_modulation");
    mSimpleTimer.setInterval(mCfg -> interval, mCfg -> critical);
    setEnabled(mCfg -> enabled);
  }

  void setup();
  void loop();
  void command(byte command);

  String getModuleName()
  {
    return FPSTR("EXPONENTIAL_POWER_MODULATION");
  }
};

#endif
