#ifndef WEBSOCKET_CONTROL_H
#define WEBSOCKET_CONTROL_H

#include <Arduino.h>
#include <WebSocketsServer.h>
#include "../../configuration/configurator.h"
#include "./websocket_control_config.h"
#include "../../utility/simpletimer.h"
#include "../base/base.hpp"
#include "../../interfaces/interfaces.hpp"

class Websocket_control : virtual public Control_module
{
private:
  FMV *mFMV;
  SimpleTimer mSimpleTimer;
  Websocket_control_config * mCfg = nullptr;
  IConnection * pIConnection = nullptr;
  void getWebsocketConnection();
protected:
  void onDisable();
  void onEvent(byte eventId, bool always = false)
  {
    if (always || isActive())
      mFMV -> moduleEvent(this, eventId);
  }
  void handleCommand(byte command, byte value);
public:
  Websocket_control(byte id, FMV *fmv, Websocket_control_config* cfg = nullptr) : Control_module(id, Modules::WEBSOCKET_CONTROL)  {
    mFMV = fmv;
    if (cfg == nullptr)
      mCfg = static_cast<Websocket_control_config*>(Configurator::Instance().createConfig(id, Configurations::WEBSOCKET_CONTROL_CONFIG));
    else
      mCfg = cfg;
    setConfig();
  }

  void setConfig()
  {
    mSimpleTimer.setName("Websocket_control");
    mSimpleTimer.setInterval(10);
    setClientTimeout("Websocket_control", 10000);
    setEnabled(mCfg -> enabled);
  }

  void setup();
  void loop();
  void command(byte command);
};

#endif
