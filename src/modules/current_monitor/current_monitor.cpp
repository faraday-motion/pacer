#include <Arduino.h>
#include "./current_monitor.h"
#include "../../configuration/default/configuration.h"

void Current_monitor::setup() {
  if (mIsSetup == false)
  {
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up "), getModuleName());
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up "), getModuleName());
    mIsSetup = true;
  }
}

void Current_monitor::loop()
{
  if (enabled())
  {
    if (mSensor == nullptr)
      mSensor = mFMV -> sensors().get(mSensorName);
    if (mSensor != nullptr)
    {
      Logger::Instance().write(LogLevel::DEBUG, getModuleName(), FPSTR("::loop"));
      if (mSensor -> hasNewValue())
      {
        if (mSensor -> getIntValue() > mMaxCurrent)
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
