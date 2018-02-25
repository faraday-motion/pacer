#include <Arduino.h>
#include "./temperature_monitor.h"
#include "../../configuration/default/configuration.h"

void Temperature_monitor::setup() {
  if (mIsSetup == false)
  {
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up "), getModuleName());
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up "), getModuleName());
    mIsSetup = true;
  }
}

void Temperature_monitor::loop()
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
        int val = mSensor -> getIntValue();
        //mFMV -> sensors().add("Temp", val);

        if (val < mMaxTemperature)
            onEvent(Events::WARNING_LOW_TEMPERATURE);
        else if (val > mMaxTemperature)
            onEvent(Events::WARNING_HIGH_TEMPERATURE);
      }
    }
  }
}

void Temperature_monitor::command(byte command)
{

}
