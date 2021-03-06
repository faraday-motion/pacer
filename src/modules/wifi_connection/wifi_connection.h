#ifndef WIFI_CONNECTION_H
#define WIFI_CONNECTION_H
#include <WiFi.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>
#include <Arduino.h>
#include "../../configuration/configurator.h"
#include "./wifi_connection_config.h"
#include "../base/modulebase.h"
#include "../../fmv.h"

class Wifi_connection : public virtual Modulebase
{
private:

  IFMV * mFMV;
  void onDisable();
  Wifi_connection_config * mCfg = nullptr;
  void setWifiOff();
  void setWifiAp();
  void setWifiStation();
protected:
  void onEvent(byte eventId, bool always = false)
  {
    mFMV -> moduleEvent(this, eventId);
  }
public:
  Wifi_connection(byte id, IFMV * fmv, Wifi_connection_config * cfg = nullptr) : Modulebase(id, Modules::WIFI_CONNECTION)  {
    mFMV = fmv;
    if (cfg == nullptr)
      mCfg = static_cast<Wifi_connection_config*>(Configurator::Instance().getConfig(id, Configurations::WIFI_CONNECTION_CONFIG));
    else
      mCfg = cfg;
    setConfig();
  }
  //static void onWiFiEvent(WiFiEvent_t event);

  enum Commands : byte {
    CONNECTION_WIFI_OFF,
    CONNECTION_WIFI_AP,
    CONNECTION_WIFI_STA,
  };

  enum Events : byte {
    CONFIGURE,
    WIFI_AP_STARTED,
    WIFI_STA_STARTED,
    WIFI_STA_ENDED,
    WIFI_DISABLED
  };

  void setConfig()
  {
    setEnabled(mCfg -> enabled);
  }

  void setup();
  void loop();
  void command(byte command);

  String getModuleName()
  {
    return FPSTR("WIFI_CONNECTION");
  }
};

#endif
