#include <Arduino.h>
#include "./voltage_monitor.h"
#include "../../configuration/default/configuration.h"

void Voltage_monitor::setup() {
  if (mIsSetup == false)
  {
    mIsSetup = true;
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up "), getModuleName());
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    onEvent(Events::CONFIGURE);
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up "), getModuleName());
  }
}

void Voltage_monitor::loop()
{
  if (enabled())
  {
      Logger::Instance().write(LogLevel::DEBUG, getModuleName(), FPSTR("::loop"));
      if (mSensor == nullptr)
        mSensor = mFMV -> sensors().getFloatSensor(mCfg -> sensorName);
      if (mSensor != nullptr && mSensor -> valueChanged())
      {
        float val = mSensor -> getValue();
        if (val > mCfg -> maxVoltage)
        {
            onEvent(Events::WARNING_VOLTAGE_HIGH);
        }
        else if (val < mCfg -> minVoltage)
        {
            onEvent(Events::WARNING_VOLTAGE_LOW);
        }
      }
  }
}

void Voltage_monitor::command(byte command)
{

}
