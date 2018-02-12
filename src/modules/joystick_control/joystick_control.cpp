#include <Arduino.h>
#include "./joystick_control.h"
#include "../../configuration/default/configuration.h"

void Joystick_control::setup() {
  if (mIsSetup == false)
  {
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up Joystick_control"));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    onEvent(Events::CONFIGURE, true);
    if (mSensorX != nullptr)
      mSensorX -> setup();
    if (mSensorY != nullptr)
      mSensorY -> setup();
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up Joystick_control"));
    mIsSetup = true;
  }
}

void Joystick_control::loop()
{
  if (enabled())
  {
    Logger::Instance().write(LogLevel::DEBUG, FPSTR("Joystick_control::loop"));
    mSensorY -> loop();
    if (mSensorY -> valueChanged())
    {
      int valueY = constrain(mSensorY -> value(), mLimitYMin, mLimitYMax);
      if (valueY > mNeutralY + mDeadbandY)
      {
        //Accelerate
        mBack = 0;
        mForward = map(valueY, mNeutralY + mDeadbandY, mLimitYMax, 0, 100);
        mOutputControl.setPower(mForward);
        setHasClient(true);
      }
      else if (valueY < mNeutralY - mDeadbandY)
      {
        //Brake
        mForward = 0;
        mBack = map(valueY, mNeutralY - mDeadbandY, mLimitYMin, 0, 100);
        mOutputControl.setBrake(mBack);
        setHasClient(true);
      }
      else
      {
        //Neutral
        mForward = 0;
        mBack = 0;
        mOutputControl.resetPower();
      }
    }

    mSensorX -> loop();
    if (mSensorX -> valueChanged())
    {
      int valueX = constrain(mSensorX -> value(), mLimitXMin, mLimitXMax);
      if (valueX > mNeutralX + mDeadbandX)
      {
        //Right
        mLeft = 0;
        mRight = map(valueX, mNeutralX + mDeadbandX, mLimitXMax, 0, 100);
        mOutputControl.setRight(mRight);
        setHasClient(true);
      }
      else if (valueX < mNeutralX - mDeadbandX)
      {
        //Left
        mRight = 0;
        mLeft = map(valueX, mNeutralX - mDeadbandX, mLimitXMin, 0, 100);
        mOutputControl.setLeft(mLeft);
        setHasClient(true);
      }
      else
      {
        //Neutral
        mRight = 0;
        mLeft = 0;
        mOutputControl.resetDirection();
      }
    }
    if (isActive())
    {
      mFMV -> sensors().add("Active", id());
      Logger::Instance().write(LogLevel::DEBUG, FPSTR("Joystick_control::Power "), String(mOutputControl.getPower()) + " " + String(mOutputControl.getBrake()));
      Logger::Instance().write(LogLevel::DEBUG, FPSTR("Joystick_control::Turning "), String(mOutputControl.getLeft()) + " " + String(mOutputControl.getRight()));
    }
    clientTimeoutCheck();
  }
}

void Joystick_control::command(byte command)
{

}

void Joystick_control::onDisable()
{
   mForward = 0;
   mBack = 0;
   mLeft = 0;
   mRight = 0;
   mOutputControl.reset();
 }
