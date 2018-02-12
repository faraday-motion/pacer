#ifndef CURRENT_MONITOR_H
#define CURRENT_MONITOR_H
#include "../../configuration/configurator.h"
#include "./current_monitor_config.h"
#include "../base/modulebase.h"
#include "../../fmv.h"
#include "../../sensors/base/sensorbase.h"

class Current_monitor : virtual public Modulebase
{
private:
  FMV *mFMV;
  Sensorbase *mSensor;
  float mMinCurrent = 0;
  float mMaxCurrent = 1023;
  Current_monitor_config* mCfg = nullptr;
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
    mSensor = mFMV -> getSensor(mCfg -> sensorId);
    setMinCurrent(mCfg -> minCurrent);
    setMaxCurrent(mCfg -> maxCurrent);
  }

  void setup();
  void loop();
  void command(byte command);

  void setMinCurrent(int min) {
    min = constrain(min, 0, 1023);
    mMinCurrent = min;
  }

  void setMaxCurrent(int max) {
    max = constrain(max, 0, 1023);
    mMaxCurrent = max;
  }

};

#endif
