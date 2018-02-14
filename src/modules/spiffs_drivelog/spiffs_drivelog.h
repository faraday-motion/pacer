#ifndef SPIFFS_DRIVELOG_H
#define SPIFFS_DRIVELOG_H
#include <Arduino.h>
#include "../../configuration/configurator.h"
#include "./spiffs_drivelog_config.h"
#include "../../utility/simpletimer.h"
#include "../base/base.hpp"
#include "../../sensors/base/sensorbase.h"
#include "../../utility/spiffs_storage.h"

class Spiffs_drivelog : virtual public Modulebase
{
  private:
    FMV *mFMV;
    void writeSensorValues();
    String mDriveLog = "";
    SimpleTimer mSimpleTimer;
    Spiffs_storage mSpiffs_storage;
    Spiffs_drivelog_config* mCfg = nullptr;
  protected:
  public:
    Spiffs_drivelog(byte id, FMV *fmv, Spiffs_drivelog_config* cfg = nullptr) : Modulebase(id, Modules::SPIFFS_DRIVELOG)  {
      mFMV = fmv;
      if (cfg == nullptr)
        mCfg = static_cast<Spiffs_drivelog_config*>(Configurator::Instance().createConfig(id, Configurations::SPIFFS_DRIVELOG_CONFIG));
      else
        mCfg = cfg;
      setConfig();
    }

    void setConfig()
    {
      mSimpleTimer.setName("Spiffs_drivelog");
      mSimpleTimer.setInterval(mCfg -> interval, mCfg -> critical);
      mDriveLog = mCfg -> driveLog;
    }

  void setup();
  void loop();
  void command(byte command);

};

#endif
