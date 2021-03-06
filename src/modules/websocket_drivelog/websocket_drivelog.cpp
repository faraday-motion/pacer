#include "./websocket_drivelog.h"
#include <vector>
#include <Arduino.h>
#include "../../utility/tools.h"
#include "../../configuration/default/configuration.h"
#include "../../configuration/configurator.h"
#include "../../interfaces/interfaces.hpp"

void Websocket_drivelog::setup() {
  if (mIsSetup == false)
  {
    mIsSetup = true;
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up "), getModuleName());
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    onEvent(Events::CONFIGURE);
    getWebsocketConnection();
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up "), getModuleName());
  }
}

void Websocket_drivelog::loop()
{
  if (enabled())
  {
    if (mSimpleTimer.check())
    {
      Logger::Instance().write(LogLevel::DEBUG, getModuleName(), FPSTR("::loop"));
      writeSensorValues();
    }
  }
}

void Websocket_drivelog::getWebsocketConnection()
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

void Websocket_drivelog::writeSensorValues()
{
  String sv = "\"sensors\":{";
  std::vector<ISensorValue*> sensors = mFMV -> sensors().all();
  for (int i=0; i<sensors.size(); i++)
  {
    sv += String("\"" + sensors[i] -> getName()) + "\":" + "\"" + String(sensors[i] -> getStringValue()) + "\"";
    if (i < sensors.size()-1)
      sv += ",";
  }
  sv += "}";
  if (pIConnection != nullptr)
    pIConnection -> send(sv);
}

void Websocket_drivelog::command(byte command)
{

}
