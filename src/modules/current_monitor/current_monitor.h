#ifndef CURRENT_MONITOR_H
#define CURRENT_MONITOR_H
#include "../../configuration/configurator.h"
#include "./current_monitor_config.h"
#include "../base/modulebase.h"
#include "../../fmv.h"

class Current_monitor : virtual public Modulebase
{
private:
  FMV * mFMV = nullptr;
  Float_sensor_value * mSensor;
  float mMinCurrent = 0;
  float mMaxCurrent = 1023;
  Current_monitor_config * mCfg = nullptr;
  String mSensorName = "";
protected:
public:
  Current_monitor(byte id, FMV *fmv, Current_monitor_config* cfg = nullptr) : Modulebase(id, Modules::CURRENT_MONITOR)  {
    mFMV = fmv;
    if (cfg == nullptr)
      mCfg = static_cast<Current_monitor_config*>(Configurator::Instance().createConfig(id, Configurations::CURRENT_MONITOR_CONFIG));
    else
      mCfg = cfg;
    setConfig();
  }

  enum Events : byte {
    WARNING_HIGH_CURRENT
  };


  void setConfig()
  {
    setMinCurrent(mCfg -> minCurrent);
    setMaxCurrent(mCfg -> maxCurrent);
    mSensorName = mCfg -> sensorName;
    setEnabled(mCfg -> enabled);
  }

  void setup();
  void loop();
  void command(byte command);

  void setMinCurrent(float min) {
    mMinCurrent = min;
  }

  void setMaxCurrent(float max) {
    mMaxCurrent = max;
  }

  String getModuleName()
  {
    return FPSTR("CURRENT_MONITOR");
  }
};

#endif
