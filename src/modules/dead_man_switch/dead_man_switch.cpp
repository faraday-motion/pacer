#include <Arduino.h>
#include "./dead_man_switch.h"
#include "../../configuration/default/configuration.h"

void Dead_man_switch::setup() {
  if (mIsSetup == false)
  {
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up "), getModuleName());
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up "), getModuleName());
    mIsSetup = true;
  }
}

void Dead_man_switch::loop()
{
  if (enabled())
  {
    if (mSensor == nullptr)
      mSensor = mFMV -> sensors().get(mSensorName);
    if (mSensor != nullptr)
    {
      Logger::Instance().write(LogLevel::DEBUG, getModuleName(), FPSTR("::loop"));
      mIsVehicleDead = mSensor -> getBoolValue();
      if (mIsVehicleDead)
        onEvent(Events::VEHICLE_ISDEAD);
      else
        onEvent(Events::VEHICLE_ISALIVE);
    }
  }
}

void Dead_man_switch::command(byte command)
{

}

void Dead_man_switch::onDisable()
{
    mIsVehicleDead = false;
    onEvent(Events::VEHICLE_ISALIVE);
}
