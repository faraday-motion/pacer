#ifndef SERIAL_CONTROL_H
#define SERIAL_CONTROL_H

#include "../../configuration/configurator.h"
#include "./serial_control_config.h"
#include "../../utility/simpletimer.h"
#include "../base/base.hpp"

class Serial_control : public virtual Control_module
{
private:
  IFMV * mFMV;
  Serial_control_config * mCfg = nullptr;
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
  Serial_control(byte id, IFMV * fmv, Serial_control_config * cfg = nullptr) : Control_module(id, Modules::SERIAL_CONTROL)  {
    mFMV = fmv;
    if (cfg == nullptr)
      mCfg = static_cast<Serial_control_config*>(Configurator::Instance().createConfig(id, Configurations::SERIAL_CONTROL_CONFIG));
    else
      mCfg = cfg;
    setConfig();
  }

  void setConfig()
  {
    setClientTimeout("Serial_control", mCfg -> resetInputTimeout);
    setEnabled(mCfg -> enabled);
  }

  bool readSerial(byte &command, byte &value);
  void setup();
  void loop();
  void command(byte command);

  String getModuleName()
  {
    return FPSTR("SERIAL_CONTROL");
  }
};

#endif
