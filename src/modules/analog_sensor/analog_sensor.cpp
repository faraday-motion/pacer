#include "./analog_sensor.h"
#include "../../utility/tools.h"

void Analog_sensor::setup() {
  if (mIsSetup == false)
  {
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up "), getModuleName());
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    pinMode(mCfg -> pin, INPUT);
    readAnalogPin();
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up "), getModuleName());
    mIsSetup = true;
  }
}

void Analog_sensor::loop()
{
  if (enabled())
  {
    if (mSimpleTimer.check())
    {
      Logger::Instance().write(LogLevel::DEBUG, getModuleName(), FPSTR("::loop"));
      readAnalogPin();
    }
  }
}

void Analog_sensor::command(byte command)
{
}

void Analog_sensor::readAnalogPin()
{
  mFMV -> sensors().setIntSensor(mCfg -> sensorName, Tools::analogReadMultiple(mCfg -> pin, 3));
}
