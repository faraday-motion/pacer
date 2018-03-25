#include <WiFi.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>
#include <Arduino.h>
#include "./bt_connection.h"
#include "../../configuration/default/configuration.h"
#include "SimpleBLE.h"

SimpleBLE ble;

void Bt_connection::setup() {
  if (mIsSetup == false)
  {
      mIsSetup = true;
      Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up "), getModuleName());
      Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
      onEvent(Events::CONFIGURE);
      btStop();
      ble.begin("ESP32 SimpleBLE");
      Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up "), getModuleName());
  }
}

void Bt_connection::loop()
{
  if (enabled())
  {

  }
}

void Bt_connection::command(byte command)
{
  Commands comm = static_cast<Commands>(command);
  if (comm == Commands::CONNECTION_BL_OFF)
  {
    btStop();
  }
  else if (comm == Commands::CONNECTION_BL_ON)
  {
    btStart();
  }
}

void Bt_connection::onDisable()
{
  btStop();
}
