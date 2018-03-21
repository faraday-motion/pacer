#ifndef POWER_LIMIT_H
#define POWER_LIMIT_H

#include "../../configuration/configurator.h"
#include "./power_limit_config.h"
#include "../base/limit_module.h"
#include "../../interfaces/interfaces.hpp"
#include "../../sensors/sensors.hpp"

class Power_limit : public Limit_module
{
private:
  IFMV * mFMV;
  void setPowerMax(byte powermax);
  Power_limit_config * mCfg = nullptr;
  SimpleTimer mSimpleTimer;
  SimpleTimer mPauseSimpleTimer;
  bool mIsVehicleDead = false;
  //Checks if the power has been reset after a dead event
  bool mIsPowerReset = true;
  Bool_sensor_value * mSensor = nullptr;
  bool mIsPause = false;
protected:
  void onDisable();
  void onEvent(byte eventId, bool always = false)
  {
    mFMV -> moduleEvent(this, eventId);
  }
public:
  Power_limit(byte id, IFMV * fmv, Power_limit_config * cfg = nullptr) : Limit_module(id, Modules::POWER_LIMIT)  {
    mFMV = fmv;
    if (cfg == nullptr)
      mCfg = static_cast<Power_limit_config*>(Configurator::Instance().createConfig(id, Configurations::POWER_LIMIT_CONFIG));
    else
      mCfg = cfg;
    setConfig();
  }

  enum Commands : byte {
    DRIVE_MODE_0,
    DRIVE_MODE_20,
    DRIVE_MODE_40,
    DRIVE_MODE_60,
    DRIVE_MODE_80,
    DRIVE_MODE_100
  };

  enum Events : byte {
    LIMIT_DEAD,
    LIMIT_DEAD_PAUSE,
    LIMIT_ALIVE,
    LIMIT_0,
    LIMIT_20,
    LIMIT_40,
    LIMIT_60,
    LIMIT_80,
    LIMIT_100,
    WARNING_SET_INPUT_NEUTRAL
  };

  void setConfig()
  {
    mSimpleTimer.setName(FPSTR("Power_limit"));
    mSimpleTimer.setInterval(15, 30);
    mPauseSimpleTimer.setName(FPSTR("Power_limit"));
    mPauseSimpleTimer.setInterval(60*1000);
    setEnabled(mCfg -> enabled);
  }

  void setup();
  void loop();
  void command(byte command);

  String getModuleName()
  {
    return FPSTR("POWER_LIMIT");
  }
};

#endif
