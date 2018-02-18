#include <Arduino.h>
#include "./force_control.h"
#include "../../configuration/default/configuration.h"

void Force_control::setup() {
  if (mIsSetup == false)
  {
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up Force_control"));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    onEvent(Events::CONFIGURE, true);
    if (mSensorY != nullptr)
      mSensorY -> setup();
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up Force_control"));
    mIsSetup = true;
  }
}

void Force_control::loop()
{
  if (enabled())
  {
    return;
    if (mSensorY != nullptr)
    {
      Logger::Instance().write(LogLevel::DEBUG, FPSTR("Force_control::loop"));
      mSensorY -> loop();
      if (mSensorY -> valueChanged())
      {
        int valueY = constrain(mSensorY -> value(), mLimitYMin, mLimitYMax);
        mFMV -> sensors().add("FORCE", valueY);
        if (valueY > mNeutralY + mDeadbandY)
        {
          //Accelerate
          mBack = 0;
          mForward = map(valueY, mNeutralY + mDeadbandY, mLimitYMax, 0, 100);
          mOutputControl.setPower(mForward);
          onEvent(Events::DRIVE_POWER);
          setHasClient(true);
        }
        else if (valueY < mNeutralY - mDeadbandY)
        {
          //Brake
          mForward = 0;
          mBack = map(valueY, mNeutralY - mDeadbandY, mLimitYMin, 0, 100);
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
      if (isActive())
      {
        mFMV -> sensors().add("Active", id());
        Logger::Instance().write(LogLevel::DEBUG, FPSTR("Force_control::Power "), String(mOutputControl.getPower()) + " " + String(mOutputControl.getBrake()));
        Logger::Instance().write(LogLevel::DEBUG, FPSTR("Force_control::Turning "), String(mOutputControl.getLeft()) + " " + String(mOutputControl.getRight()));
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