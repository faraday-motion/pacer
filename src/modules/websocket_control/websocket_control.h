#ifndef WEBSOCKET_CONTROL_H
#define WEBSOCKET_CONTROL_H

#include <Arduino.h>
#include <WebSocketsServer.h>
#include "../../configuration/configurator.h"
#include "./websocket_control_config.h"
#include "../../utility/simpletimer.h"
#include "../base/input_control.h"
#include "../../fmv.h"
#include "../../interfaces/interfaces.hpp"
#include "./websocket_writer.h"

class Websocket_control : virtual public Input_control
{
private:
  FMV *mFMV;
  SimpleTimer mSimpleTimer;
  Websocket_control_config* mCfg = nullptr;
  byte mSubscribers[];
  WebSocketsServer* mWebSocketsServer = nullptr;
  Websocket_writer* pWriter = nullptr;
  int mPort = 0;
  byte mClientMax = 0;
  void onWsEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);
protected:
  void onDisable();
  void onEvent(byte eventId, bool always = false)
  {
    if (always || isActive())
      mFMV -> moduleEvent(this, eventId);
  }
  void handleCommand(byte command, byte value);
public:
  Websocket_control(byte id, FMV *fmv, Websocket_control_config* cfg = nullptr) : Input_control(id, Modules::WEBSOCKET_CONTROL, fmv)  {
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
    mPort = mCfg -> port;
    mClientMax = mCfg -> clientMax;
    setClientTimeout("Websocket_control", 10000);
    setEnabled(mCfg -> enabled);
  }

  void setup();
  void loop();
  void command(byte command);
};

#endif
