#ifndef TEMPERATURE_MONITOR_H
#define TEMPERATURE_MONITOR_H
#include "../../configuration/configurator.h"
#include "./temperature_monitor_config.h"
#include "../base/modulebase.h"
#include "../../interfaces/interfaces.hpp"
#include "../../sensors/sensors.hpp"

class Temperature_monitor : public virtual Modulebase
{
private:
  IFMV * mFMV;
  Float_sensor_value * mSensor;
  Temperature_monitor_config * mCfg = nullptr;
protected:
  void onEvent(byte eventId, bool always = false)
  {
    mFMV -> moduleEvent(this, eventId);
  }
public:
  Temperature_monitor(byte id, IFMV * fmv, Temperature_monitor_config * cfg = nullptr) : Modulebase(id, Modules::TEMPERATURE_MONITOR)  {
    mFMV = fmv;
    if (cfg == nullptr)
      mCfg = static_cast<Temperature_monitor_config*>(Configurator::Instance().createConfig(id, Configurations::TEMPERATURE_MONITOR_CONFIG));
    else
      mCfg = cfg;
    setConfig();
  }

  enum Events : byte {
    WARNING_LOW_TEMPERATURE,
    WARNING_HIGH_TEMPERATURE
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
    return FPSTR("TEMPERATURE_MONITOR");
  }
};

#endif
