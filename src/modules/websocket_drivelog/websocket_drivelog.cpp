#include "./websocket_drivelog.h"
#include <vector>
#include <Arduino.h>
#include "../../sensors/sensor_value.h"
#include "../../utility/tools.h"
#include "../../configuration/default/configuration.h"
#include "../../configuration/configurator.h"

void Websocket_drivelog::setup() {
  if (mIsSetup == false)
  {
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up Websocket_drivelog"));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    getWebsocketConnection();
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up Websocket_drivelog"));
    mIsSetup = true;
  }
}

void Websocket_drivelog::loop()
{
  if (enabled())
  {
    if (mSimpleTimer.check())
    {
      Logger::Instance().write(LogLevel::DEBUG, FPSTR("Websocket_drivelog::loop"));
      writeSensorValues();
    }
  }
}

void Websocket_drivelog::getWebsocketConnection()
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

void Websocket_drivelog::writeSensorValues()
{
  String sv = "{ \"drive_log\": {";
  std::vector<Sensor_value*> sensors = mFMV -> sensors().all();
  for (int i=0; i< sensors.size(); i++)
  {
    sv += String("\"" + sensors[i] -> getName()) + "\":" + "\"" + String(sensors[i] -> getValue()) + "\"";
    if (i < sensors.size()-1)
      sv += ",";
  }
  sv += "}}";
  if (pIConnection != nullptr)
    pIConnection -> send(sv);
}

void Websocket_drivelog::command(byte command)
{

}
