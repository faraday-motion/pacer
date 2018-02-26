#include "./spiffs_drivelog.h"
#include <vector>
#include <Arduino.h>
#include "../../utility/tools.h"
#include "../../configuration/default/configuration.h"
#include "../../configuration/configurator.h"

void Spiffs_drivelog::setup() {
  if (mIsSetup == false)
  {
    mIsSetup = true;
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up "), getModuleName());
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    Configurator::Instance().initializeSpiff();
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up "), getModuleName());
  }
}

void Spiffs_drivelog::loop()
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

void Spiffs_drivelog::writeSensorValues()
{
  String sv = "";
  std::vector<ISensorvalue*> sensors = mFMV -> sensors().all();
  for (int i=0; i< sensors.size(); i++)
  {
    sv += String(sensors[i] -> getName()) + ":" + String(sensors[i] -> getStringValue()) + ", ";
  }
  sv += ";\n";
//  sv += ";";
  mSpiffs_storage.append(mDriveLog, sv);
}

void Spiffs_drivelog::command(byte command)
{

}
