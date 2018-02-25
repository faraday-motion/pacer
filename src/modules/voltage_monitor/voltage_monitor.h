#ifndef VOLTAGE_MONITOR_H
#define VOLTAGE_MONITOR_H

#include <Arduino.h>
#include "../../configuration/configurator.h"
#include "./voltage_monitor_config.h"
#include "../base/modulebase.h"
#include "../../fmv.h"

class Voltage_monitor : virtual public Modulebase
{
private:
  FMV *mFMV;
  Sensor_value * mSensor;
  float mMinVoltage = 0;
  float mMaxVoltage = 1023;
  Voltage_monitor_config* mCfg = nullptr;
  String mSensorName = "";
protected:
  void onEvent(byte eventId)
  {
    mFMV -> moduleEvent(this, eventId);
  }
public:
  Voltage_monitor(byte id, FMV *fmv, Voltage_monitor_config* cfg = nullptr) : Modulebase(id, Modules::VOLTAGE_MONITOR)  {
    mFMV = fmv;
    if (cfg == nullptr)
      mCfg = static_cast<Voltage_monitor_config*>(Configurator::Instance().createConfig(id, Configurations::VOLTAGE_MONITOR_CONFIG));
    else
      mCfg = cfg;
    setConfig();
  }

  enum Events : byte {
      WARNING_VOLTAGE_HIGH = 1,
      WARNING_VOLTAGE_LOW = 2
  };

  void setConfig()
  {
    setMinVoltage(mCfg -> minVoltage);
    setMaxVoltage(mCfg -> maxVoltage);
    mSensorName = mCfg -> sensorName;
    setEnabled(mCfg -> enabled);
  }

  void setup();
  void loop();
  void command(byte command);

  void setMaxVoltage(int max) {
    max = constrain(max, 0, 1023);
    mMaxVoltage = max;
  }

  void setMinVoltage(int min) {
    min = constrain(min, 0, 1023);
    mMinVoltage = min;
  }

  String getModuleName()
  {
    return FPSTR("VOLTAGE_MONITOR");
  }
};

#endif
