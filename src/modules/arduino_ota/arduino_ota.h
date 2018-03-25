#ifndef ARDUINO_OTA_H
#define ARDUINO_OTA_H
#include <Arduino.h>
#include "../../configuration/configurator.h"
#include "./arduino_ota_config.h"
#include "../base/modulebase.h"
#include "../../interfaces/interfaces.hpp"
#include "../../utility/simpletimer.h"

class Arduino_ota : public virtual Modulebase
{
private:
  IFMV * mFMV = nullptr;
  void onDisable();
  SimpleTimer mSimpleTimer;
  Arduino_ota_config * mCfg = nullptr;
protected:
  void onEvent(byte eventId, bool always = false)
  {
    mFMV -> moduleEvent(this, eventId);
  }
public:
  Arduino_ota(byte id, IFMV * fmv, Arduino_ota_config * cfg = nullptr) : Modulebase(id, Modules::ARDUINO_OTA)  {
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
    CONFIGURE
  };

  void setConfig()
  {
    mSimpleTimer.setName("Arduino_ota");
    mSimpleTimer.setInterval(10);
    setEnabled(mCfg -> enabled);
  }

  void setup();
  void loop();
  void command(byte command);

  String getModuleName()
  {
    return FPSTR("ARDUINO_OTA");
  }
};

#endif
