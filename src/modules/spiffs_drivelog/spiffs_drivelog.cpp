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
    onEvent(Events::CONFIGURE);
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
  std::vector<ISensorValue*> sensors = mFMV -> sensors().all();
  for (int i=0; i< sensors.size(); i++)
  {
    sv += String(sensors[i] -> getName()) + ":" + String(sensors[i] -> getStringValue()) + ", ";
  }
  sv += ";\n";
  //TODO Add MAX SIZE
  mSpiffs_storage.append(mCfg -> driveLog, sv, 700000);
}

void Spiffs_drivelog::command(byte command)
{
  if (enabled())
  {
    Logger::Instance().write(LogLevel::INFO, FPSTR("Spiffs_drivelog::command: "), String(command));
    Commands comm = static_cast<Commands>(command);
    if (comm == Commands::CLEAR_LOG)
    {
      mSpiffs_storage.remove(mCfg -> driveLog);
    }
  }
}
