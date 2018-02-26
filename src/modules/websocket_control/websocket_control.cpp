
#include <Arduino.h>
#include <ArduinoJson.h>
#include "./websocket_control.h"
#include "../../configuration/default/configuration.h"
#include "../../configuration/configurator.h"
#include "../../enums/enums.hpp"
#include "../../utility/tools.h"
#include "../../utility/spiffs_storage.h"

using namespace std;
using namespace std::placeholders;

//Websocket control gives a reset when combined with the new led driver
void Websocket_control::setup() {
  if (mIsSetup == false)
  {
    mIsSetup = true;
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up "), getModuleName());
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    onEvent(CONFIGURE, true);
    addWebsocketReciever();
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up "), getModuleName());
  }
}

void Websocket_control::addWebsocketReciever()
{
  //Find the websocket connection module
  if (pIConnection == nullptr)
  {
    Modulebase * mb = mFMV -> modules().getByType(Modules::WEBSOCKET_CONNECTION);
    if (mb != nullptr)
    {
      Connection_module * conn = static_cast<Connection_module*>(mb);
      pIConnection = static_cast<IConnection*>(conn);
      if (pIConnection != nullptr)
      {
        Logger::Instance().write(LogLevel::INFO, FPSTR("Websocket_control::addReciever"));
        pIConnection -> addReciever(this);
      }
    }
  }
}

void Websocket_control::loop()
{
  if (enabled())
  {
    if (mSimpleTimer.check())
    {
      Logger::Instance().write(LogLevel::DEBUG, getModuleName(), FPSTR("::loop"));
      clientTimeoutCheck();
    }
  }
}

void Websocket_control::command(byte command)
{

}

void Websocket_control::onDisable()
{
   mOutputControl.reset();
   onEvent(Events::CONTROL_NONE);
}
