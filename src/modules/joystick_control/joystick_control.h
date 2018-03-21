#ifndef JOYSTICK_CONTROL_H
#define JOYSTICK_CONTROL_H

#include "../../configuration/configurator.h"
#include "./joystick_control_config.h"
#include "../base/base.hpp"
#include "../../interfaces/interfaces.hpp"
#include "../../sensors/sensors.hpp"

class Joystick_control : public Control_module
{
private:
  IFMV * mFMV = nullptr;
  Int_sensor_value * mSensorX = nullptr;
  Int_sensor_value * mSensorY = nullptr;
  byte mForward = 0;
  byte mBack = 0;
  byte mLeft = 0;
  byte mRight = 0;
  Joystick_control_config * mCfg = nullptr;
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
  Joystick_control(byte id, IFMV * fmv, Joystick_control_config * cfg = nullptr) : Control_module(id, Modules::JOYSTICK_CONTROL)  {
    mFMV = fmv;
    if (cfg == nullptr)
      mCfg = static_cast<Joystick_control_config*>(Configurator::Instance().createConfig(id, Configurations::JOYSTICK_CONTROL_CONFIG));
    else
      mCfg = cfg;
    setConfig();
  }

  void setConfig()
  {
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
