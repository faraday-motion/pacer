#ifndef WEBSOCKET_SERVER_LOG_H
#define WEBSOCKET_SERVER_LOG_H

#include <Arduino.h>
#include "../../utility/tools.h"
#include "../../configuration/configurator.h"
#include "./websocket_server_log_config.h"
#include "../base/base.hpp"
#include "../../interfaces/interfaces.hpp"

class Websocket_server_log : public virtual Log_module
{
private:
  Websocket_server_log_config* mCfg = nullptr;
  IFMV * mFMV = nullptr;
  IConnection * pIConnection = nullptr;

  void getWebsocketConnection()
  {
    //Find the websocket connection module
    if (pIConnection == nullptr)
    {
      IModule * mb = mFMV -> modules().getByType(Modules::WEBSOCKET_CONNECTION);
      if (mb != nullptr)
      {
        Connection_module * conn = static_cast<Connection_module*>(mb);
        pIConnection = static_cast<IConnection*>(conn);
      }
    }
  }
protected:
  void onEvent(byte eventId, bool always = false)
  {
    mFMV -> moduleEvent(this, eventId);
  }
public:
  Websocket_server_log(byte id, IFMV * fmv = nullptr, Websocket_server_log_config * mfg = nullptr) : Log_module(id, Modules::WEBSOCKET_SERVER_LOG){
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

  void write(LogLevel level, String message)
  {
    if (enabled() && level >= getLevel())
    {
      message = ", \"content\":\"" + message + "\"";
      if (pIConnection != nullptr)
        pIConnection -> send(message);
    }
  }

  void setup()
  {
    if (mIsSetup == false)
    {
      mIsSetup = true;
      Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up "), getModuleName());
      Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
      onEvent(Events::CONFIGURE);
      getWebsocketConnection();
      Logger::Instance().addLog(this);
      Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up "), getModuleName());
    }
  }

  enum Events : byte {
    CONFIGURE
  };
  
  void loop()
  {
  }

  void command(byte command)
  {
  }

  String getModuleName()
  {
    return FPSTR("WEBSOCKET_SERVER_LOG");
  }
};

#endif
