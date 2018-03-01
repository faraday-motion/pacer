#ifndef WEBSOCKET_CONNECTION_H
#define WEBSOCKET_CONNECTION_H

#include <Arduino.h>
#include <WebSocketsServer.h>
#include "../../configuration/configurator.h"
#include "./websocket_connection_config.h"
#include "../../utility/simpletimer.h"
#include "../base/connection_module.h"
#include "../../fmv.h"
#include "../../interfaces/interfaces.hpp"

class Websocket_connection : public Connection_module
{
private:
  FMV *mFMV;
  SimpleTimer mSimpleTimer;
  Websocket_connection_config* mCfg = nullptr;
  WebSocketsServer* mWebSocketsServer = nullptr;
  std::vector<uint8_t> pClients;
  int mPort = 0;
  byte mClientMax = 0;
  void onWsEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);
protected:
  void onDisable();
  void onEvent(byte eventId, bool always = false)
  {
    mFMV -> moduleEvent(this, eventId);
  }
public:
  Websocket_connection(byte id, FMV *fmv, Websocket_connection_config* cfg = nullptr) : Connection_module(id, Modules::WEBSOCKET_CONNECTION)  {
    mFMV = fmv;
    if (cfg == nullptr)
      mCfg = static_cast<Websocket_connection_config*>(Configurator::Instance().createConfig(id, Configurations::WEBSOCKET_CONNECTION_CONFIG));
    else
      mCfg = cfg;
    setConfig();
  }

  void send(String message);

  void addClient(uint8_t clientId)
  {
    if (pClients.size() < mClientMax)
      pClients.push_back(clientId);
  }

  void removeClient(uint8_t clientId)
  {
    int toDelete = -1;
    bool deleted = false;
    for (int i=0; i<pClients.size(); i++)
    {
      if (pClients[i] == clientId)
      {
        toDelete = i;
        break;
      }
    }
    pClients.erase(pClients.begin() + toDelete);
  }

  void setConfig()
  {
    mSimpleTimer.setName("Websocket_control");
    mSimpleTimer.setInterval(10);
    mPort = mCfg -> port;
    mClientMax = mCfg -> clientMax;
    setEnabled(mCfg -> enabled);
  }

  enum Commands : byte {
  };

  enum Events : byte {
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
