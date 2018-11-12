#ifndef VOLTAGE_MONITOR_H
#define VOLTAGE_MONITOR_H

#include <Arduino.h>
#include "../../configuration/configurator.h"
#include "./voltage_monitor_config.h"
#include "../base/modulebase.h"
#include "../../interfaces/interfaces.hpp"
#include "../../sensors/sensors.hpp"

class Voltage_monitor : public Modulebase
{
private:
  IFMV * mFMV;
  Float_sensor_value * mSensor;
  Voltage_monitor_config* mCfg = nullptr;
protected:
  void onEvent(byte eventId, bool always = false)
  {
    mFMV -> moduleEvent(this, eventId);
  }
public:
  Voltage_monitor(byte id, IFMV * fmv, Voltage_monitor_config * cfg = nullptr) : Modulebase(id, Modules::VOLTAGE_MONITOR)  {
    mFMV = fmv;
    if (cfg == nullptr)
      mCfg = static_cast<Voltage_monitor_config*>(Configurator::Instance().getConfig(id, Configurations::VOLTAGE_MONITOR_CONFIG));
    else
      mCfg = cfg;
    setConfig();
  }

  enum Events : byte {
    CONFIGURE,
    WARNING_VOLTAGE_HIGH = 1,
    WARNING_VOLTAGE_LOW = 2
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
    return FPSTR("VOLTAGE_MONITOR");
  }
};

#endif
