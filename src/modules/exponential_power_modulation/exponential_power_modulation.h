#ifndef EXPONENTIAL_POWER_MODULATION_H
#define EXPONENTIAL_POWER_MODULATION_H

#include <Arduino.h>
#include "../base/base.hpp"
#include "../../configuration/configurator.h"
#include "./exponential_power_modulation_config.h"
#include "../../utility/simpletimer.h"

class Exponential_power_modulation : virtual public Modulation_module
{
private:
  FMV * mFMV = nullptr;
  SimpleTimer mSimpleTimer;
  byte mPreviousPower = 0;
  byte mPreviousBrake = 0;
  float mSmoothAlpha = 0;
  Exponential_power_modulation_config* mCfg = nullptr;
  byte calculateOutput(byte previousPower, byte targetPower);
protected:
  void onDisable();
public:
  Exponential_power_modulation(byte id, FMV* fmv, Exponential_power_modulation_config* cfg = nullptr) : Modulation_module(id, Modules::EXPONENTIAL_POWER_MODULATION)  {
    mFMV = fmv;
    if (cfg == nullptr)
      mCfg = static_cast<Exponential_power_modulation_config*>(Configurator::Instance().createConfig(id, Configurations::EXPONENTIAL_POWER_MODULATION_CONFIG));
    else
      mCfg = cfg;

    setConfig();
  }

  void setConfig()
  {
    mSimpleTimer.setName("Exponential_power_modulation");
    mSimpleTimer.setInterval(mCfg -> interval, mCfg -> critical);
    mSmoothAlpha = mCfg -> smoothAlpha;
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
