#ifndef CONTROL_PRIORITY_H
#define CONTROL_PRIORITY_H

#include "../../configuration/configurator.h"
#include "./control_priority_config.h"
#include "../../utility/simpletimer.h"
#include "../base/base.hpp"
//#include "../../fmv.h"

class Control_priority : virtual public Modulebase
{
private:
  FMV *mFMV;
  SimpleTimer mSimpleTimer;
  Control_priority_config* mCfg = nullptr;
  int mPriority1 = -1;
  int mPriority2 = -1;
  int mPriority3 = -1;
  int mPriority4 = -1;
  int mPriority5 = -1;
protected:
  void onDisable();
  void onEvent(byte eventId)
  {
    mFMV -> moduleEvent(this, eventId);
  }
public:
  Control_priority(byte id, FMV *fmv, Control_priority_config* cfg = nullptr) : Modulebase(id, Modules::CONTROL_PRIORITY)  {
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
    mPriority1 = mCfg -> priority1;
    mPriority2 = mCfg -> priority2;
    mPriority3 = mCfg -> priority3;
    mPriority4 = mCfg -> priority4;
    mPriority5 = mCfg -> priority5;
    setEnabled(mCfg -> enabled);
  }

  void setup();
  void loop();
  void command(byte command);
};

#endif
