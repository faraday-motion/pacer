#ifndef FORCE_CONTROL_H
#define FORCE_CONTROL_H

#include "../../configuration/configurator.h"
#include "./force_control_config.h"
#include "../base/base.hpp"
#include "../../sensors/sensors.hpp"

class Force_control : public Control_module
{
private:
  IFMV * mFMV = nullptr;
  Int_sensor_value * mSensorY = nullptr;
  byte mForward = 0;
  byte mBack = 0;
  Force_control_config * mCfg = nullptr;
protected:
  void onDisable();
  void onEvent(byte eventId, bool always = false)
  {
    Logger::Instance().write(LogLevel::INFO, getModuleName(), FPSTR(" onEvent eventId: ") + String(eventId));
    if (always || isActive())
    {
      Logger::Instance().write(LogLevel::INFO, getModuleName(), FPSTR(" onEvent moduleEvent eventId: ") + String(eventId));
      mFMV -> moduleEvent(this, eventId);
    }
  }
public:
  Force_control(byte id, IFMV * fmv, Force_control_config* cfg = nullptr) : Control_module(id, Modules::FORCE_CONTROL)  {
    mFMV = fmv;
    if (cfg == nullptr)
      mCfg = static_cast<Force_control_config*>(Configurator::Instance().createConfig(id, Configurations::FORCE_CONTROL_CONFIG));
    else
      mCfg = cfg;
    setConfig();
  }

  void setConfig()
  {
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
