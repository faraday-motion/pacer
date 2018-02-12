#include <Arduino.h>
#include "./current_monitor.h"
#include "../../configuration/default/configuration.h"

void Current_monitor::setup() {
  if (mIsSetup == false)
  {
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up Current_monitor"));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    if (mSensor != nullptr)
      mSensor -> setup();
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up Current_monitor"));
    mIsSetup = true;
  }
}

void Current_monitor::loop()
{
  if (enabled())
  {
      if (mSensor != nullptr)
      {
        Logger::Instance().write(LogLevel::DEBUG, FPSTR("Current_monitor::loop"));
        mSensor -> loop();
        if (mSensor -> hasNewValue())
        {
          if (mSensor -> value() > mMaxCurrent)
          {
              onEvent(Events::WARNING_HIGH_CURRENT);
          }
        }
    }
  }
}

void Current_monitor::command(byte command)
{

}
