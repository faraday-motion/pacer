#ifndef FORCE_CONTROL_H
#define FORCE_CONTROL_H

#include "../../configuration/configurator.h"
#include "./force_control_config.h"
#include "../base/base.hpp"

class Force_control : virtual public Control_module
{
private:
  FMV * mFMV = nullptr;
  Sensor_value * mSensorY = nullptr;
  int mDeadbandY = 50;
  int mNeutralY = 512;
  int mLimitYMin = 0;
  int mLimitYMax = 1023;
  bool mInvertY = false;
  byte mForward = 0;
  byte mBack = 0;
  Force_control_config* mCfg = nullptr;
  String mSensorName = "";
protected:
  void onDisable();
  void onEvent(byte eventId, bool always = false)
  {
    if (always || isActive())
      mFMV -> moduleEvent(this, eventId);
  }
public:
  Force_control(byte id, FMV *fmv, Force_control_config* cfg = nullptr) : Control_module(id, Modules::FORCE_CONTROL)  {
    mFMV = fmv;
    if (cfg == nullptr)
      mCfg = static_cast<Force_control_config*>(Configurator::Instance().createConfig(id, Configurations::FORCE_CONTROL_CONFIG));
    else
      mCfg = cfg;
    setConfig();
  }

  void setConfig()
  {
    mDeadbandY = mCfg -> deadbandY;
    mNeutralY = mCfg -> neutralY;
    mLimitYMin = mCfg -> limitYMin;
    mLimitYMax = mCfg -> limitYMax;
    mInvertY = mCfg -> invertY;
    mSensorName = mCfg -> sensorName;
    setClientTimeout("Force_control", 10000);
    setEnabled(mCfg -> enabled);
  }

  void setup();
  void loop();
  void command(byte command);

  String getModuleName()
  {
    return FPSTR("FORCE_CONTROL");
  }
};

#endif
