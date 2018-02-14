#ifndef ARDUINO_OTA_H
#define ARDUINO_OTA_H
#include <Arduino.h>
#include "../../configuration/configurator.h"
#include "./arduino_ota_config.h"
#include "../base/modulebase.h"
#include "../../fmv.h"
#include "../../sensors/base/sensorbase.h"

class Arduino_ota : virtual public Modulebase
{
private:
  FMV *mFMV;
  void onDisable();
  SimpleTimer mSimpleTimer;
  Arduino_ota_config* mCfg = nullptr;
  int mPort = 0;
  String mHostName = "";
  String mPassword = "";
protected:
  void onEvent(byte eventId)
  {
    mFMV -> moduleEvent(this, eventId);
  }
public:
  Arduino_ota(byte id, FMV *fmv, Arduino_ota_config* cfg = nullptr) : Modulebase(id, Modules::ARDUINO_OTA)  {
    mFMV = fmv;
    if (cfg == nullptr)
      mCfg = static_cast<Arduino_ota_config*>(Configurator::Instance().createConfig(id, Configurations::ARDUINO_OTA_CONFIG));
    else
      mCfg = cfg;
    setConfig();
  }

  enum Commands : byte {
  };

  enum Events : byte {
  };

  void setConfig()
  {
    mSimpleTimer.setName("Arduino_ota");
    mSimpleTimer.setInterval(10);
    mPort = mCfg -> port;
    mHostName = mCfg -> hostName;
    mPassword = mCfg -> password;
  }

  void setup();
  void loop();
  void command(byte command);

};

#endif
