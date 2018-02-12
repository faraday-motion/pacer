#include <Arduino.h>
#include "./voltage_monitor.h"
#include "../../configuration/default/configuration.h"

void Voltage_monitor::setup() {
  if (mIsSetup == false)
  {
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up Voltage_monitor"));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    mSensor -> setup();
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up Voltage_monitor"));
    mIsSetup = true;
  }
}

void Voltage_monitor::loop()
{
  if (enabled())
  {
      Logger::Instance().write(LogLevel::DEBUG, FPSTR("Voltage_monitor::loop"));
      mSensor -> loop();
      if (mSensor -> hasNewValue())
      {
        float val = mSensor -> value();
        mFMV -> sensors().add("Voltage", val);
        if (val > mMaxVoltage)
        {
            onEvent(Events::WARNING_VOLTAGE_HIGH);
        }
        else if (val < mMinVoltage)
        {
            onEvent(Events::WARNING_VOLTAGE_LOW);
        }
      }
  }
}

void Voltage_monitor::command(byte command)
{

}
