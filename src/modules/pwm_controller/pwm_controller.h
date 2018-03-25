#ifndef PWM_CONTROLLER_H
#define PWM_CONTROLLER_H

#include <vector>
#include <Arduino.h>
#include "../../configuration/configurator.h"
#include "./pwm_controller_config.h"
#include "./pwm_controller_wheel_decorator.h"
#include "../base/power_module.h"
#include "../../interfaces/interfaces.hpp"
#include "../../utility/tools.h"

class Pwm_controller : public virtual Power_module
{
private:
  IFMV * mFMV;
  SimpleTimer mSimpleTimer;
  Pwm_controller_config * mCfg = nullptr;
  std::vector<Pwm_controller_wheel_decorator*> wheelDecorators;
  //All pin numbers are allowed,but only pins 2,4,12-19,21-23,25-27,32-33 are recommended.
protected:
  void onDisable();
  void onEvent(byte eventId, bool always = false)
  {
    mFMV -> moduleEvent(this, eventId);
  }
public:
  Pwm_controller(byte id, IFMV * fmv, Pwm_controller_config * cfg = nullptr) : Power_module(id, Modules::PWM_CONTROLLER)  {
    mFMV = fmv;
    if (cfg == nullptr)
      mCfg = static_cast<Pwm_controller_config*>(Configurator::Instance().createConfig(id, Configurations::PWM_CONTROLLER_CONFIG));
    else
      mCfg = cfg;
    setConfig();
  }

  enum Events : byte {
    CONFIGURE
  };

  void setConfig()
  {
    mSimpleTimer.setName("Pwm_controller");
    mSimpleTimer.setInterval(25, 50);
    setEnabled(mCfg -> enabled);
  }

  void setup();
  void loop();
  void command(byte command);

  String getModuleName()
  {
    return FPSTR("PWM_CONTROLLER");
  }
};

#endif
