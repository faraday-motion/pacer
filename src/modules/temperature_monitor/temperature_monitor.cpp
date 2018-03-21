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
      mSensor = mFMV -> sensors().getFloatSensor(mCfg -> sensorName);
    if (mSensor != nullptr)
    {
      Logger::Instance().write(LogLevel::DEBUG, getModuleName(), FPSTR("::loop"));
      if (mSensor -> valueChanged())
      {
        float val = mSensor -> getValue();
        if (val < mCfg -> minTemperature)
            onEvent(Events::WARNING_LOW_TEMPERATURE);
        else if (val > mCfg -> maxTemperature)
            onEvent(Events::WARNING_HIGH_TEMPERATURE);
      }
    }
  }
}

void Temperature_monitor::command(byte command)
{

}
