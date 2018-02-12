#ifndef DEAD_MAN_SWITCH_H
#define DEAD_MAN_SWITCH_H
#include "../../configuration/configurator.h"
#include "./dead_man_switch_config.h"
#include "../base/modulebase.h"
#include "../../fmv.h"
#include "../../sensors/base/sensorbase.h"

class Dead_man_switch : virtual public Modulebase
{
private:
  FMV *mFMV;
  Sensorbase *mSensor;
  bool mIsVehicleDead = false;
  Dead_man_switch_config* mCfg = nullptr;
protected:
    void onDisable();
    void onEvent(byte eventId)
    {
      mFMV -> moduleEvent(this, eventId);
    }
public:
  Dead_man_switch(byte id, FMV *fmv, Dead_man_switch_config* cfg = nullptr) : Modulebase(id, Modules::DEAD_MAN_SWITCH)  {
    mFMV = fmv;
    if (cfg == nullptr)
      mCfg = static_cast<Dead_man_switch_config*>(Configurator::Instance().createConfig(id, Configurations::DEAD_MAN_SWITCH_CONFIG));
    else
      mCfg = cfg;
    setConfig();
  }

  enum Events : byte {
    VEHICLE_ISDEAD,
    VEHICLE_ISALIVE
  };

  void setConfig()
  {
    mSensor = mFMV -> getSensor(mCfg -> sensorId);
  }

  void setup();
  void loop();
  void command(byte command);
  //bool isVehicleDead();
};

#endif
