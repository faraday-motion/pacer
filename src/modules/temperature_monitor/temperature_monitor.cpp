#include <Arduino.h>
#include "./temperature_monitor.h"
#include "../../configuration/default/configuration.h"

void Temperature_monitor::setup() {
  if (mIsSetup == false)
  {
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up Temperature_monitor"));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    if (mSensor != nullptr)
      mSensor -> setup();
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up Temperature_monitor"));
    mIsSetup = true;
  }
}

void Temperature_monitor::loop()
{
  if (enabled())
  {
    if (mSensor != nullptr)
    {
      Logger::Instance().write(LogLevel::DEBUG, FPSTR("Temperature_monitor::loop"));
      mSensor -> loop();
      if (mSensor -> hasNewValue())
      {
        float val = mSensor -> value();
          mFMV -> sensors().add("Temp", val);

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
