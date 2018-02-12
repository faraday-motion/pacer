#ifndef NTP_TIMESERVICE_H
#define NTP_TIMESERVICE_H
#include <Arduino.h>
#include <WiFi.h>
#include "../../configuration/configurator.h"
#include "./ntp_timeservice_config.h"
#include "../base/modulebase.h"
#include "../../fmv.h"

class Ntp_timeservice : virtual public Modulebase
{
private:
  FMV *mFMV;
  String mTimeServer = "";
  String mTimeZone = "";
  String mDateTimeFormat = "";
  struct tm mTimeInfo;
  Ntp_timeservice_config* mCfg = nullptr;
  void getTime();
protected:

public:
  Ntp_timeservice(byte id, FMV *fmv, Ntp_timeservice_config* cfg = nullptr) : Modulebase(id, Modules::NTP_TIMESERVICE)  {
    mFMV = fmv;
    if (cfg == nullptr)
      mCfg = static_cast<Ntp_timeservice_config*>(Configurator::Instance().createConfig(id, Configurations::NTP_TIMESERVICE_CONFIG));
    else
      mCfg = cfg;
    setConfig();
  }

  enum Commands : byte {
    GET_TIME
  };

  enum Events : byte {

  };

  void setConfig()
  {
    mTimeServer = mCfg -> timeServer;
    mTimeZone = mCfg -> timeZone;
    mDateTimeFormat = mCfg -> dateTimeFormat;
  }

  void setup();
  void loop();
  void command(byte command);

};

#endif
