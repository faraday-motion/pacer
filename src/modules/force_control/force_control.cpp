#include <Arduino.h>
#include "./force_control.h"
#include "../../configuration/default/configuration.h"

void Force_control::setup() {
  if (mIsSetup == false)
  {
    mIsSetup = true;
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up "), getModuleName());
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    onEvent(Events::CONFIGURE, true);
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up "), getModuleName());
  }
}

void Force_control::loop()
{
  if (enabled())
  {
    if (mSensorY != nullptr)
    {
      Logger::Instance().write(LogLevel::DEBUG, getModuleName(), FPSTR("::loop"));
      if (mSensorY == nullptr)
        mSensorY = mFMV -> sensors().getIntSensor(mCfg -> sensorName);
      if (mSensorY != nullptr && mSensorY -> valueChanged())
      {
        int valueY = constrain(mSensorY -> getValue(), mCfg -> limitYMin, mCfg -> limitYMax);
        if (valueY > mCfg -> neutralY + mCfg -> deadbandY)
        {
          //Accelerate
          mBack = 0;
          mForward = map(valueY, mCfg -> neutralY + mCfg -> deadbandY, mCfg -> limitYMax, 0, 100);
          mOutputControl.setPower(mForward);
          onEvent(Events::DRIVE_POWER);
          setHasClient(true);
        }
        else if (valueY < mCfg -> neutralY - mCfg -> deadbandY)
        {
          //Brake
          mForward = 0;
          mBack = map(valueY, mCfg -> neutralY - mCfg -> deadbandY, mCfg -> limitYMin, 0, 100);
          mOutputControl.setBrake(mBack);
          onEvent(Events::DRIVE_BRAKE);
          setHasClient(true);
        }
        else
        {
          //Neutral
          mForward = 0;
          mBack = 0;
          mOutputControl.resetPower();
          onEvent(Events::DRIVE_NEUTRAL);
        }
      }
    }
    clientTimeoutCheck();
  }
}

void Force_control::command(byte command)
{

}

void Force_control::onDisable()
{
   mForward = 0;
   mBack = 0;
   mOutputControl.reset();
   onEvent(Events::CONTROL_NONE);
}
