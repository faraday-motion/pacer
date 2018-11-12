#ifndef NTP_TIMESERVICE_H
#define NTP_TIMESERVICE_H
#include <Arduino.h>
#include <WiFi.h>
#include "../../configuration/configurator.h"
#include "./ntp_timeservice_config.h"
#include "../base/modulebase.h"
#include "../../utility/simpletimer.h"
#include "../../interfaces/interfaces.hpp"

class Ntp_timeservice : public virtual Modulebase
{
private:
  IFMV * mFMV = nullptr;
  struct tm mTimeInfo;
  Ntp_timeservice_config * mCfg = nullptr;
  void getTime();
  void updateTime();
  SimpleTimer mSimpleTimer;
  bool mGotTimeInfo = false;
protected:
  void onEvent(byte eventId, bool always = false)
  {
    mFMV -> moduleEvent(this, eventId);
  }
public:
  Ntp_timeservice(byte id, IFMV * fmv, Ntp_timeservice_config * cfg = nullptr) : Modulebase(id, Modules::NTP_TIMESERVICE)  {
    mFMV = fmv;
    if (cfg == nullptr)
      mCfg = static_cast<Ntp_timeservice_config*>(Configurator::Instance().getConfig(id, Configurations::NTP_TIMESERVICE_CONFIG));
    else
      mCfg = cfg;
    setConfig();
  }

  enum Commands : byte {
    GET_TIME
  };

  enum Events : byte {
    CONFIGURE
  };

  void setConfig()
  {
    mSimpleTimer.setInterval(1000);
    setEnabled(mCfg -> enabled);
  }

  void setup();
  void loop();
  void command(byte command);

  String getModuleName()
  {
    return FPSTR("NTP_TIMESERVICE");
  }
};

#endif
