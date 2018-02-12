#ifndef WIFI_SIMPLE_CONTROL_H
#define WIFI_SIMPLE_CONTROL_H

#include <WiFi.h>
#include "../../configuration/configurator.h"
#include "./wifi_simple_control_config.h"
#include "../base/input_control.h"
#include "../../fmv.h"
#include "../../sensors/base/sensorbase.h"

class Wifi_simple_control : virtual public Input_control
{
private:
  FMV *mFMV;
  byte mForward = 0;
  byte mBack = 0;
  Wifi_simple_control_config* mCfg = nullptr;
  WiFiServer*  server;
  WiFiClient client;
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
    if (always || isActive())
      mFMV -> moduleEvent(this, eventId);
  }
public:
  Wifi_simple_control(byte id, FMV *fmv, Wifi_simple_control_config* cfg = nullptr) : Input_control(id, Modules::WIFI_SIMPLE_CONTROL, fmv)  {
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
};

#endif
