#ifndef WEBSOCKET_SERVER_LOG_H
#define WEBSOCKET_SERVER_LOG_H

#include <Arduino.h>
#include "../base/logbase.h"
#include "../../utility/tools.h"
#include "../../configuration/configurator.h"
#include "./websocket_server_log_config.h"
#include "../../fmv.h"
#include "../../interfaces/interfaces.hpp"
#include "../../modules/modules.hpp"

class Websocket_server_log : public Logbase
{
private:
  Websocket_server_log_config* mCfg = nullptr;
  FMV * mFMV = nullptr;
  IConnection * pIConnection = nullptr;
  void getWebsocketConnection()
  {
    //Find the websocket connection module
    if (pIConnection == nullptr)
    {
      Modulebase * mb = mFMV -> modules().get(Modules::WEBSOCKET_CONNECTION);
      if (mb != nullptr)
      {
        Connection_module * conn = static_cast<Connection_module*>(mb);
        pIConnection = static_cast<IConnection*>(conn);
      }
    }
  }

protected:
public:
  Websocket_server_log(byte id, FMV *fmv = nullptr, Websocket_server_log_config* mfg = nullptr) : Logbase(id){
    mFMV = fmv;
    if (mfg == nullptr)
      mCfg = static_cast<Websocket_server_log_config*>(Configurator::Instance().createConfig(id, Configurations::WEBSOCKET_SERVER_LOG_CONFIG));
    else
      mCfg = mfg;
    //getWebsocketConnection();
    setConfig();
  };

  void setConfig()
  {
    setLevel(mCfg -> logLevel);
    setEnabled(mCfg -> enabled);
  }

  void write(LogLevel level, String message)
  {
    if (enabled() && level >= getLevel())
    {
      //getWebsocketConnection();
      if (pIConnection != nullptr)
        pIConnection -> send(message);
    }
  }
};
#endif
