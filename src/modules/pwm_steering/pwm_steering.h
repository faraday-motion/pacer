#ifndef PWM_STEERING_H
#define PWM_STEERING_H

#include <vector>
#include <Arduino.h>
#include "../../configuration/configurator.h"
#include "./pwm_steering_config.h"
#include "./pwm_steering_wheel_decorator.h"
#include "../base/power_module.h"
#include "../../interfaces/interfaces.hpp"

class Pwm_steering : public virtual Power_module
{
private:
  IFMV * mFMV;
  SimpleTimer mSimpleTimer;
  Pwm_steering_config * mCfg = nullptr;
  std::vector<Pwm_steering_wheel_decorator*> wheelDecorators;
protected:
  void onDisable();
  void onEvent(byte eventId, bool always = false)
  {
    mFMV -> moduleEvent(this, eventId);
  }
public:
  Pwm_steering(byte id, IFMV * fmv, Pwm_steering_config * cfg = nullptr) : Power_module(id, Modules::PWM_STEERING)  {
    mFMV = fmv;
    if (cfg == nullptr)
      mCfg = static_cast<Pwm_steering_config*>(Configurator::Instance().createConfig(id, Configurations::PWM_STEERING_CONFIG));
    else
      mCfg = cfg;
    setConfig();
  }

  enum Events : byte {
    CONFIGURE
  };

  void setConfig()
  {
    mSimpleTimer.setName("Pwm_steering");
    mSimpleTimer.setInterval(25, 50);
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
