#ifndef POWER_LIMIT_H
#define POWER_LIMIT_H

#include "../../configuration/configurator.h"
#include "./power_limit_config.h"
#include "../base/limit_module.h"
#include "../../fmv.h"
#include "../../sensors/base/sensorbase.h"

class Power_limit : virtual public Limit_module
{
private:
  FMV *mFMV;
  void setPowerMax(byte powermax);
  Power_limit_config* mCfg = nullptr;
  SimpleTimer mSimpleTimer;
  bool mIsVehicleDead = false;
  //Checks if the power has been reset after a dead event
  bool mIsPowerReset = true;
protected:
  void onDisable();
  void onEvent(byte eventId)
  {
    mFMV -> moduleEvent(this, eventId);
  }
public:
  Power_limit(byte id, FMV *fmv, Power_limit_config* cfg = nullptr) : Limit_module(id, Modules::POWER_LIMIT)  {
    mFMV = fmv;
    if (cfg == nullptr)
      mCfg = static_cast<Power_limit_config*>(Configurator::Instance().createConfig(id, Configurations::POWER_LIMIT_CONFIG));
    else
      mCfg = cfg;
    setConfig();
  }

  enum Commands : byte {
    VEHICLE_DEAD,
    VEHICLE_ALIVE,
    DRIVE_MODE_0,
    DRIVE_MODE_20,
    DRIVE_MODE_40,
    DRIVE_MODE_60,
    DRIVE_MODE_80,
    DRIVE_MODE_100
  };

  enum Events : byte {
    LIMIT_DEAD,
    LIMIT_ALIVE,
    LIMIT_0,
    LIMIT_20,
    LIMIT_40,
    LIMIT_60,
    LIMIT_80,
    LIMIT_NONE,
    WARNING_SET_INPUT_NEUTRAL
  };

  void setConfig()
  {
    mSimpleTimer.setName("Power_limit");
    mSimpleTimer.setInterval(15, 30);
  }

  void setup();
  void loop();
  void command(byte command);
};

#endif
