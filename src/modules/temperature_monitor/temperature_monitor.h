#ifndef TEMPERATURE_MONITOR_H
#define TEMPERATURE_MONITOR_H
#include "../../configuration/configurator.h"
#include "./temperature_monitor_config.h"
#include "../base/modulebase.h"
#include "../../fmv.h"
#include "../../sensors/base/sensorbase.h"

class Temperature_monitor : virtual public Modulebase
{
private:
  FMV *mFMV;
  Sensorbase *mSensor;
  float mMinTemperature = 0;
  float mMaxTemperature = 1023;
  Voltage_monitor_config* mCfg = nullptr;
protected:
public:
  Temperature_monitor(byte id, FMV *fmv, Voltage_monitor_config* cfg = nullptr) : Modulebase(id, Modules::TEMPERATURE_MONITOR)  {
    mFMV = fmv;
    if (cfg == nullptr)
      mCfg = static_cast<Voltage_monitor_config*>(Configurator::Instance().createConfig(id, Configurations::TEMPERATURE_MONITOR_CONFIG));
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
    mSensor = mFMV -> getSensor(mCfg -> sensorId);
    setMinTemperature(mCfg -> minVoltage);
    setMaxTemperature(mCfg -> maxVoltage);
  }

  void setup();
  void loop();
  void command(byte command);

  void setMinTemperature(int min) {
    min = constrain(min, 0, 1023);
    mMinTemperature = min;
  }

  void setMaxTemperature(int max) {
    max = constrain(max, 0, 1023);
    mMaxTemperature = max;
  }

};

#endif
