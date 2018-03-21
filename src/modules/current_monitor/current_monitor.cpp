#include <Arduino.h>
#include "./current_monitor.h"
#include "../../configuration/default/configuration.h"

void Current_monitor::setup() {
  if (mIsSetup == false)
  {
    mIsSetup = true;
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up "), getModuleName());
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up "), getModuleName());
  }
}

void Current_monitor::loop()
{
  if (enabled())
  {
    if (mSensor == nullptr)
      mSensor = mFMV -> sensors().getFloatSensor(mCfg -> sensorName);
    if (mSensor != nullptr)
    {
      Logger::Instance().write(LogLevel::DEBUG, getModuleName(), FPSTR("::loop"));
      if (mSensor -> valueChanged())
      {
        if (mSensor -> getValue() > mCfg -> maxCurrent)
        {
            onEvent(Events::WARNING_HIGH_CURRENT);
        }
      }
    }
  }
}

void Current_monitor::command(byte command)
{

}
