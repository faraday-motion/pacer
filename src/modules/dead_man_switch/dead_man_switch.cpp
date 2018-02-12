#include <Arduino.h>
#include "./dead_man_switch.h"
#include "../../configuration/default/configuration.h"

void Dead_man_switch::setup() {
  if (mIsSetup == false)
  {
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up Dead_man_switch"));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    if (mSensor != nullptr)
      mSensor -> setup();
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up Dead_man_switch"));
    mIsSetup = true;
  }
}

void Dead_man_switch::loop()
{
  if (enabled())
  {
    if (mSensor != nullptr)
    {
      Logger::Instance().write(LogLevel::DEBUG, FPSTR("Dead_man_switch::loop"));
      mSensor -> loop();
      if (mSensor -> valueChanged())
      {
        mIsVehicleDead = (bool)mSensor -> value();
        if (mIsVehicleDead)
          onEvent(Events::VEHICLE_ISDEAD);
        else
          onEvent(Events::VEHICLE_ISALIVE);
        mFMV -> sensors().add("Dead", mIsVehicleDead);
      }
      //Logger::Instance().write(LogLevel::DEBUG, FPSTR("Dead_man_switch: "), String(mIsVehicleDead));
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
