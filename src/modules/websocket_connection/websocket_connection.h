#ifndef WEBSOCKET_CONNECTION_H
#define WEBSOCKET_CONNECTION_H

#include <Arduino.h>
#include <WebSocketsServer.h>
#include "../../configuration/configurator.h"
#include "./websocket_connection_config.h"
#include "../../utility/simpletimer.h"
#include "../base/connection_module.h"
#include "../../interfaces/interfaces.hpp"

class Websocket_connection : public virtual Connection_module
{
private:
  IFMV * mFMV;
  SimpleTimer mSimpleTimer;
  Websocket_connection_config* mCfg = nullptr;
  WebSocketsServer * mWebSocketsServer = nullptr;
  std::vector<uint8_t> pClients;
  void onWsEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);
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
      mCfg = static_cast<Websocket_connection_config*>(Configurator::Instance().createConfig(id, Configurations::WEBSOCKET_CONNECTION_CONFIG));
    else
      mCfg = cfg;
    setConfig();
  }

  void send(String message);

  void addClient(uint8_t clientId)
  {
    if (pClients.size() < mCfg -> clientMax)
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
