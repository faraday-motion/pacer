#include "boolean_sensor.h"

void Boolean_sensor::setup() {
  if (mIsSetup == false)
  {
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up "), getModuleName());
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    onEvent(Events::CONFIGURE);
    pinMode(mCfg -> pin, INPUT_PULLDOWN);
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
  if (mCfg -> invert)
    mFMV -> sensors().setBoolSensor(mCfg -> sensorName, !Tools::digitalReadMultiple(mCfg -> pin, 3));
  else
    mFMV -> sensors().setBoolSensor(mCfg -> sensorName, Tools::digitalReadMultiple(mCfg -> pin, 3));
}
