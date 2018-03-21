#ifndef CONTROL_PRIORITY_H
#define CONTROL_PRIORITY_H

#include "../../configuration/configurator.h"
#include "./control_priority_config.h"
#include "../../utility/simpletimer.h"
#include "../base/base.hpp"
#include "../../interfaces/interfaces.hpp"


class Control_priority : public virtual Modulebase
{
private:
  IFMV * mFMV = nullptr;
  SimpleTimer mSimpleTimer;
  Control_priority_config * mCfg = nullptr;
protected:
  void onDisable();
  void onEvent(byte eventId, bool always = false)
  {
    mFMV -> moduleEvent(this, eventId);
  }
public:
  Control_priority(byte id, IFMV * fmv, Control_priority_config * cfg = nullptr) : Modulebase(id, Modules::CONTROL_PRIORITY)  {
    mFMV = fmv;
    if (cfg == nullptr)
      mCfg = static_cast<Control_priority_config*>(Configurator::Instance().createConfig(id, Configurations::CONTROL_PRIORITY_CONFIG));
    else
      mCfg = cfg;
    setConfig();
  }

  enum Commands : byte {

  };

  enum Events : byte {
    ACTIVE_PRIORITY1,
    ACTIVE_PRIORITY2,
    ACTIVE_PRIORITY3,
    ACTIVE_PRIORITY4,
    ACTIVE_PRIORITY5
  };

  void setConfig()
  {
    mSimpleTimer.setName("Control_priority");
    mSimpleTimer.setInterval(mCfg -> interval);
    setEnabled(mCfg -> enabled);
  }

  void setup();
  void loop();
  void command(byte command);

  String getModuleName()
  {
    return FPSTR("CONTROL_PRIORITY");
  }
};

#endif
