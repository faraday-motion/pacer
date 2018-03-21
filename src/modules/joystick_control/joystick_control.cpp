#include <Arduino.h>
#include "./joystick_control.h"
#include "../../configuration/default/configuration.h"

void Joystick_control::setup() {
  if (mIsSetup == false)
  {
    mIsSetup = true;
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up "), getModuleName());
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    onEvent(Events::CONFIGURE, true);
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up "), getModuleName());
  }
}

void Joystick_control::loop()
{
  if (enabled())
  {
    Logger::Instance().write(LogLevel::DEBUG, getModuleName(), FPSTR("::loop"));
    if (mSensorY == nullptr)
      mSensorY = mFMV -> sensors().getIntSensor(mCfg -> sensorNameY);
    if (mSensorY != nullptr && mSensorY -> valueChanged())
    {
      int valueY = constrain(mSensorY -> getValue(), mCfg -> limitYMin, mCfg -> limitYMax);
      if (valueY > mCfg -> neutralY + mCfg -> deadbandY)
      {
        //Accelerate
        mBack = 0;
        mForward = map(valueY, mCfg -> neutralY + mCfg -> deadbandY, mCfg -> limitYMax, 0, 100);
        mOutputControl.setPower(mForward);
        setHasClient(true);
      }
      else if (valueY < mCfg -> neutralY - mCfg -> deadbandY)
      {
        //Brake
        mForward = 0;
        mBack = map(valueY, mCfg -> neutralY - mCfg -> deadbandY, mCfg -> limitYMin, 0, 100);
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

    if (mSensorX == nullptr)
      mSensorX = mFMV -> sensors().getIntSensor(mCfg -> sensorNameX);
    if (mSensorX != nullptr && mSensorX -> valueChanged())
    {
      int valueX = constrain(mSensorX -> getValue(), mCfg -> limitXMin, mCfg -> limitXMax);
      if (valueX > mCfg -> neutralX + mCfg -> deadbandX)
      {
        //Right
        mLeft = 0;
        mRight = map(valueX, mCfg -> neutralX + mCfg -> deadbandX, mCfg -> limitXMax, 0, 100);
        mOutputControl.setRight(mRight);
        setHasClient(true);
      }
      else if (valueX < mCfg -> neutralX - mCfg -> deadbandX)
      {
        //Left
        mRight = 0;
        mLeft = map(valueX, mCfg -> neutralX - mCfg -> deadbandX, mCfg -> limitXMin, 0, 100);
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
