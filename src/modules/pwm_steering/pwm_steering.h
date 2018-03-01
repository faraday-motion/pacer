#ifndef PWM_STEERING_H
#define PWM_STEERING_H

#include <vector>
#include <Arduino.h>
#include "../../configuration/configurator.h"
#include "./pwm_steering_config.h"
#include "./pwm_steering_wheel_decorator.h"
#include "../base/power_module.h"
#include "../../fmv.h"

class Pwm_steering : virtual public Power_module
{
private:
  FMV *mFMV;
  SimpleTimer mSimpleTimer;
  Pwm_steering_config* mCfg = nullptr;
  std::vector<Pwm_steering_wheel_decorator*> wheelDecorators;
  //All pin numbers are allowed,but only pins 2,4,12-19,21-23,25-27,32-33 are recommended.
  byte mServoPins[4] = {0, 0, 0, 0};
  byte mMinAngle = 0;
  byte mMaxAngle = 0;
  byte mNeutral = 0;
protected:
  void onDisable();
  void onEvent(byte eventId, bool always = false)
  {
    mFMV -> moduleEvent(this, eventId);
  }
public:
  Pwm_steering(byte id, FMV *fmv, Pwm_steering_config* cfg = nullptr) : Power_module(id, Modules::PWM_STEERING)  {
    mFMV = fmv;
    if (cfg == nullptr)
      mCfg = static_cast<Pwm_steering_config*>(Configurator::Instance().createConfig(id, Configurations::PWM_STEERING_CONFIG));
    else
      mCfg = cfg;
    setConfig();
  }

  enum Events : byte {
  };

  void setConfig()
  {
    mSimpleTimer.setName("Pwm_steering");
    mSimpleTimer.setInterval(25, 50);
    Tools::copyArray(mCfg -> servoPins, mServoPins, 4);
    mMinAngle = mCfg -> minAngle;
    mMaxAngle = mCfg -> maxAngle;
    mNeutral =  mCfg -> neutral;
    setEnabled(mCfg -> enabled);
  }

  void setup();
  void loop();
  void command(byte command);

  String getModuleName()
  {
    return FPSTR("PWM_STEERING");
  }
};

#endif
