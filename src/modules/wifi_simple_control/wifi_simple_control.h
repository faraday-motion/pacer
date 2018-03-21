#ifndef WIFI_SIMPLE_CONTROL_H
#define WIFI_SIMPLE_CONTROL_H

#include <WiFi.h>
#include "../../configuration/configurator.h"
#include "./wifi_simple_control_config.h"
#include "../base/base.hpp"
#include "../../utility/simpletimer.h"
#include "../../interfaces/interfaces.hpp"

class Wifi_simple_control : public virtual Control_module
{
private:
  IFMV * mFMV;
  byte mForward = 0;
  byte mBack = 0;
  Wifi_simple_control_config * mCfg = nullptr;
  WiFiServer * pServer;
  WiFiClient mClient;
  SimpleTimer mSimpleTimerConnection;
  SimpleTimer mSimpleTimerInput;
  void readInput();
  void processInput(byte input);
  void handleClientConnections();
protected:
  void onEnable();
  void onDisable();
  void onEvent(byte eventId, bool always = false)
  {
    Logger::Instance().write(LogLevel::INFO, getModuleName(), FPSTR(" onEvent eventId: ") + String(eventId));
    if (always || isActive())
    {
      Logger::Instance().write(LogLevel::INFO, getModuleName(), FPSTR(" onEvent moduleEvent eventId: ") + String(eventId));
      mFMV -> moduleEvent(this, eventId);
    }
  }
public:
  Wifi_simple_control(byte id, IFMV * fmv, Wifi_simple_control_config * cfg = nullptr) : Control_module(id, Modules::WIFI_SIMPLE_CONTROL)  {
    mFMV = fmv;
    if (cfg == nullptr)
      mCfg = static_cast<Wifi_simple_control_config*>(Configurator::Instance().createConfig(id, Configurations::WIFI_SIMPLE_CONTROL_CONFIG));
    else
      mCfg = cfg;
    setConfig();
  }

  void setConfig()
  {
    mSimpleTimerConnection.setName("SimpleTimerConnection");
    mSimpleTimerConnection.setInterval(150, 250);
    mSimpleTimerInput.setName("SimpleTimerInput");
    mSimpleTimerInput.setInterval(50, 75);
    setClientTimeout("Wifi_simple_control", mCfg -> inputTimeout);
    setEnabled(mCfg -> enabled);
  }

  void setup();
  void loop();
  void command(byte command);

  String getModuleName()
  {
    return FPSTR("WIFI_SIMPLE_CONTROL");
  }
};

#endif
