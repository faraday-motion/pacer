#ifndef CURRENT_MONITOR_H
#define CURRENT_MONITOR_H

#include "../../configuration/configurator.h"
#include "./current_monitor_config.h"
#include "../base/modulebase.h"
#include "../../interfaces/interfaces.hpp"
#include "../../sensors/sensors.hpp"

class Current_monitor : public Modulebase
{
private:
  IFMV * mFMV = nullptr;
  Float_sensor_value * mSensor;
  Current_monitor_config * mCfg = nullptr;
protected:
  void onEvent(byte eventId, bool always = false)
  {
    mFMV -> moduleEvent(this, eventId);
  }
public:
  Current_monitor(byte id, IFMV * fmv, Current_monitor_config * cfg = nullptr) : Modulebase(id, Modules::CURRENT_MONITOR)  {
    mFMV = fmv;
    if (cfg == nullptr)
      mCfg = static_cast<Current_monitor_config*>(Configurator::Instance().getConfig(id, Configurations::CURRENT_MONITOR_CONFIG));
    else
      mCfg = cfg;
    setConfig();
  }

  enum Events : byte {
    CONFIGURE,
    WARNING_HIGH_CURRENT
  };


  void setConfig()
  {
    setEnabled(mCfg -> enabled);
  }

  void setup();
  void loop();
  void command(byte command);

  String getModuleName()
  {
    return FPSTR("CURRENT_MONITOR");
  }
};

#endif
