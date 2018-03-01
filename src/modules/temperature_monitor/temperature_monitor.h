#ifndef TEMPERATURE_MONITOR_H
#define TEMPERATURE_MONITOR_H
#include "../../configuration/configurator.h"
#include "./temperature_monitor_config.h"
#include "../base/modulebase.h"
#include "../../fmv.h"

class Temperature_monitor : virtual public Modulebase
{
private:
  FMV *mFMV;
  Float_sensor_value * mSensor;
  float mMinTemperature = 0;
  float mMaxTemperature = 1023;
  Temperature_monitor_config* mCfg = nullptr;
  String mSensorName = "";
protected:
  void onEvent(byte eventId, bool always = false)
  {
    mFMV -> moduleEvent(this, eventId);
  }
public:
  Temperature_monitor(byte id, FMV *fmv, Temperature_monitor_config* cfg = nullptr) : Modulebase(id, Modules::TEMPERATURE_MONITOR)  {
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
    setMinTemperature(mCfg -> minTemperature);
    setMaxTemperature(mCfg -> maxTemperature);
    mSensorName = mCfg -> sensorName;
    setEnabled(mCfg -> enabled);
  }

  void setup();
  void loop();
  void command(byte command);

  void setMinTemperature(float min) {
    mMinTemperature = min;
  }

  void setMaxTemperature(float max) {
    mMaxTemperature = max;
  }

  String getModuleName()
  {
    return FPSTR("TEMPERATURE_MONITOR");
  }
};

#endif
