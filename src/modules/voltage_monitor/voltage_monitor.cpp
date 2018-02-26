#include <Arduino.h>
#include "./voltage_monitor.h"
#include "../../configuration/default/configuration.h"

void Voltage_monitor::setup() {
  if (mIsSetup == false)
  {
    mIsSetup = true;
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up "), getModuleName());
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up "), getModuleName());
  }
}

void Voltage_monitor::loop()
{
  if (enabled())
  {
      Logger::Instance().write(LogLevel::DEBUG, getModuleName(), FPSTR("::loop"));
      if (mSensor == nullptr)
        mSensor = mFMV -> sensors().getFloatSensor(mSensorName);
      if (mSensor != nullptr && mSensor -> valueChanged())
      {
        float val = mSensor -> getValue();
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
