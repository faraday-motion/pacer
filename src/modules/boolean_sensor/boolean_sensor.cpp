#include "boolean_sensor.h"

void Boolean_sensor::setup() {
  if (mIsSetup == false)
  {
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up "), getModuleName());
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    pinMode(mPin, INPUT_PULLDOWN);
    readBooleanPin();
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up "), getModuleName());
    mIsSetup = true;
  }
}

void Boolean_sensor::loop()
{
  if (enabled())
  {
    if (mSimpleTimer.check())
    {
      Logger::Instance().write(LogLevel::DEBUG, getModuleName(), FPSTR("::loop"));
      readBooleanPin();
    }
  }
}

void Boolean_sensor::command(byte command)
{
}


void Boolean_sensor::readBooleanPin()
{
  if (!mInvert)
    mFMV -> sensors().setBoolSensor(mSensorName, !Tools::digitalReadMultiple(mPin, 3));
  else
    mFMV -> sensors().setBoolSensor(mSensorName, Tools::digitalReadMultiple(mPin, 3));
}
