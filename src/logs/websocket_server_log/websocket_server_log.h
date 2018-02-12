#ifndef WEBSOCKET_SERVER_LOG_H
#define WEBSOCKET_SERVER_LOG_H

#include <Arduino.h>
#include <WebSocketsServer.h>
#include "../base/logbase.h"
#include "../../utility/tools.h"
#include "../../utility/spiffs_storage.h"
#include "../../configuration/configurator.h"
#include "./websocket_server_log_config.h"
#include "../../fmv.h"
#include "../../interfaces/interfaces.hpp"

using namespace std;
using namespace std::placeholders;

class Websocket_server_log : public Logbase
{
private:
  Websocket_server_log_config* mCfg = nullptr;
  FMV * mFMV = nullptr;
protected:
public:
  Websocket_server_log(byte id, FMV *fmv = nullptr, Websocket_server_log_config* mfg = nullptr) : Logbase(id){
    mFMV = fmv;
    if (mfg == nullptr)
      mCfg = static_cast<Websocket_server_log_config*>(Configurator::Instance().createConfig(id, Configurations::WEBSOCKET_SERVER_LOG_CONFIG));
    else
      mCfg = mfg;
    setConfig();
  };

  void setConfig()
  {
    setLevel(mCfg -> logLevel);
    setEnabled(mCfg -> enabled);
  }

  void setWriter(IWrite * writer)
  {
    pWriter = writer;
  }

  void write(LogLevel level, String message)
  {
    if (enabled() && level >= getLevel())
    {
      if (pWriter != nullptr)
        pWriter -> write(message);
    }
  }
};
#endif
