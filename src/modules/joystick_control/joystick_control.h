#ifndef JOYSTICK_CONTROL_H
#define JOYSTICK_CONTROL_H

#include "../../configuration/configurator.h"
#include "./joystick_control_config.h"
#include "../base/base.hpp"

class Joystick_control : virtual public Control_module
{
private:
  FMV * mFMV = nullptr;
  Sensor_value * mSensorX = nullptr;
  Sensor_value * mSensorY = nullptr;
  int mDeadbandY = 50;
  int mNeutralY = 512;
  int mLimitYMin = 0;
  int mLimitYMax = 1023;
  int mDeadbandX = 50;
  int mNeutralX = 512;
  int mLimitXMin = 0;
  int mLimitXMax = 1023;
  bool mInvertY = false;
  bool mInvertX = false;
  byte mForward = 0;
  byte mBack = 0;
  byte mLeft = 0;
  byte mRight = 0;
  Joystick_control_config* mCfg = nullptr;
  String mSensorNameX = "";
  String mSensorNameY = "";
protected:
  void onDisable();
  void onEvent(byte eventId, bool always = false)
  {
    if (always || isActive())
      mFMV -> moduleEvent(this, eventId);
  }
public:
  Joystick_control(byte id, FMV *fmv, Joystick_control_config* cfg = nullptr) : Control_module(id, Modules::JOYSTICK_CONTROL)  {
    mFMV = fmv;
    if (cfg == nullptr)
      mCfg = static_cast<Joystick_control_config*>(Configurator::Instance().createConfig(id, Configurations::JOYSTICK_CONTROL_CONFIG));
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
    mSensorNameX = mCfg -> sensorNameX;
    mSensorNameY = mCfg -> sensorNameY;
    setClientTimeout("Joystick_control", 10000);
    setEnabled(mCfg -> enabled);
  }

  void setup();
  void loop();
  void command(byte command);

  String getModuleName()
  {
    return FPSTR("JOYSTICK_CONTROL");
  }
};

#endif
