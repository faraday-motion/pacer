#ifndef WEBSOCKET_CONNECTION_H
#define WEBSOCKET_CONNECTION_H

#include <Arduino.h>
#include <WebSocketsServer.h>
#include "../../configuration/configurator.h"
#include "./websocket_connection_config.h"
#include "../../utility/simpletimer.h"
#include "../base/connection_module.h"
#include "../../interfaces/interfaces.hpp"

typedef struct {
  uint32_t lastContact;
  bool pinged;
  bool connected;
} WebsocketClientPingStatus;

class Websocket_connection : public virtual Connection_module
{
private:
  IFMV * mFMV;
  SimpleTimer mSimpleTimer;
  SimpleTimer mPingpongSimpleTimer;
  Websocket_connection_config * mCfg = nullptr;
  WebSocketsServer * mWebSocketsServer = nullptr;
  void onWsEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);
  void sendInternal(String message);
  WebsocketClientPingStatus mClientStatus[WEBSOCKETS_SERVER_CLIENT_MAX];
  void checkClients();
  void pingClient(int num);
  void evictClient(int num);
  void clientConnected(int num);
  void clientDisconnected(int num);
  void clientContact(int num);
  bool handleMaxClients(int num);
protected:
  void onDisable();
  void onEvent(byte eventId, bool always = false)
  {
    mFMV -> moduleEvent(this, eventId);
  }
public:
  Websocket_connection(byte id, IFMV * fmv, Websocket_connection_config * cfg = nullptr) : Connection_module(id, Modules::WEBSOCKET_CONNECTION)  {
    mFMV = fmv;
    if (cfg == nullptr)
      mCfg = static_cast<Websocket_connection_config*>(Configurator::Instance().getConfig(id, Configurations::WEBSOCKET_CONNECTION_CONFIG));
    else
      mCfg = cfg;
    setConfig();
  }

  void send(String message);

  void setConfig()
  {
    mSimpleTimer.setName("Websocket_connection");
    mSimpleTimer.setInterval(5, 50);
    mPingpongSimpleTimer.setInterval(500);
    setEnabled(mCfg -> enabled);
  }

  enum Commands : byte {
  };

  enum Events : byte {
    CONFIGURE
  };

  void setup();
  void loop();
  void command(byte command);

  String getModuleName()
  {
    return FPSTR("WEBSOCKET_CONNECTION");
  }
};

#endif
