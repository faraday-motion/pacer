#ifndef WEB_UPDATE_H
#define WEB_UPDATE_H

#include <ESP32WebServer.h>
#include <Update.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>
#include <Arduino.h>
#include "../../configuration/configurator.h"
#include "./web_update_config.h"
#include "../base/modulebase.h"
#include "../../interfaces/interfaces.hpp"
#include "../../utility/simpletimer.h"

class Web_update : public Modulebase
{
private:
  IFMV * mFMV;
  ESP32WebServer * mWebserver;
  void setRoutes();
  String getContentType(String filename);
  bool handleFileStream(String path);
  SimpleTimer mSimpleTimer;
  Web_update_config * mCfg = nullptr;
protected:
  void onEvent(byte eventId, bool always = false)
  {
    mFMV -> moduleEvent(this, eventId);
  }
public:
  Web_update(byte id, IFMV * fmv, Web_update_config * cfg = nullptr) : Modulebase(id, Modules::WEB_UPDATE)  {
    mFMV = fmv;
    if (cfg == nullptr)
      mCfg = static_cast<Web_update_config*>(Configurator::Instance().createConfig(id, Configurations::WEB_UPDATE_CONFIG));
    else
      mCfg = cfg;
    setConfig();
  }

  void setConfig()
  {
    mSimpleTimer.setName("Web_update");
    mSimpleTimer.setInterval(25, 50);
    setEnabled(mCfg -> enabled);
  }

  void setup();
  void loop();
  void command(byte command);
  String getModuleName()
  {
    return FPSTR("WEB_UPDATE");
  }
};

#endif
