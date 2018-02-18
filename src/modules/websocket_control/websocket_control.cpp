
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

void Websocket_control::setup() {
  if (mIsSetup == false)
  {
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up Websocket_control"));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    onEvent(CONFIGURE, true);
    getWebsocketConnection();
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up Websocket_control"));
    mIsSetup = true;
  }
}

void Websocket_control::getWebsocketConnection()
{
  //Find the websocket connection module
  if (pIConnection == nullptr)
  {
    Modulebase * mb = mFMV -> modules().get(Modules::WEBSOCKET_CONNECTION);
    if (mb != nullptr)
    {
      Connection_module * conn = static_cast<Connection_module*>(mb);
      pIConnection = static_cast<IConnection*>(conn);
      if (pIConnection != nullptr)
      {
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
      Logger::Instance().write(LogLevel::DEBUG, FPSTR("Websocket_control::loop"));

      if (isActive())
      {
        mFMV -> sensors().add("active", id());
        Logger::Instance().write(LogLevel::DEBUG, FPSTR("Websocket_control::Power "), String(mOutputControl.getPower()) + " " + String(mOutputControl.getBrake()));
        Logger::Instance().write(LogLevel::DEBUG, FPSTR("Websocket_control::Turning "), String(mOutputControl.getLeft()) + " " + String(mOutputControl.getRight()));
      }
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
