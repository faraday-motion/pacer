#ifndef FORCE_CONTROL_H
#define FORCE_CONTROL_H

#include "../../configuration/configurator.h"
#include "./force_control_config.h"
#include "../base/input_control.h"
#include "../../fmv.h"
#include "../../sensors/base/sensorbase.h"

class Force_control : virtual public Input_control
{
private:
  FMV *mFMV;
  Sensorbase *mSensorY = nullptr;
  int mDeadbandY = 50;
  int mNeutralY = 512;
  int mLimitYMin = 0;
  int mLimitYMax = 1023;
  bool mInvertY = false;
  byte mForward = 0;
  byte mBack = 0;
  Force_control_config* mCfg = nullptr;
protected:
  void onDisable();
  void onEvent(byte eventId, bool always = false)
  {
    if (always || isActive())
      mFMV -> moduleEvent(this, eventId);
  }
public:
  Force_control(byte id, FMV *fmv, Force_control_config* cfg = nullptr) : Input_control(id, Modules::FORCE_CONTROL, fmv)  {
    mFMV = fmv;
    if (cfg == nullptr)
      mCfg = static_cast<Force_control_config*>(Configurator::Instance().createConfig(id, Configurations::FORCE_CONTROL_CONFIG));
    else
      mCfg = cfg;
    setConfig();
  }

  void setConfig()
  {
    mSensorY = mFMV -> getSensor(mCfg -> sensorId);
    mDeadbandY = mCfg -> deadbandY;
    mNeutralY = mCfg -> neutralY;
    mLimitYMin = mCfg -> limitYMin;
    mLimitYMax = mCfg -> limitYMax;
    mInvertY = mCfg -> invertY;
    setClientTimeout("Force_control", 10000);
    setEnabled(mCfg -> enabled);
  }

  void setup();
  void loop();
  void command(byte command);
};

#endif
