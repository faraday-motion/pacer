#include "./exponential_power_modulation.h"
#include <Arduino.h>
#include "../../utility/simpletimer.h"
#include "../../configuration/default/configuration.h"
#include "../base/input_control.h"

void Exponential_power_modulation::setup() {
  if (mIsSetup == false)
  {
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up Exponential_power_modulation"));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up Exponential_power_modulation"));
    mIsSetup = true;
  }
}

void Exponential_power_modulation::loop()
{
  if (enabled())
  {
    if (mSimpleTimer.check())
    {
      Logger::Instance().write(LogLevel::DEBUG, FPSTR("Exponential_power_modulation::loop"));
      Modulebase* mb = mFMV -> modules().getActiveControl();
      if (mb != nullptr)
      {
        Input_control* ic = static_cast<Input_control*>(mb);
        mInputControl = Vehiclecontrol(ic -> getOutputControl());
        if (mInputControl.getPower() > 0)
          mOutputControl.setPower(calculateOutput(mPreviousPower, mInputControl.getPower()));
        else if (mInputControl.getBrake() > 0)
          mOutputControl.setBrake(calculateOutput(mPreviousBrake, mInputControl.getBrake()));
        else
          mOutputControl.resetPower();

        if (mInputControl.getLeft() > 0)
          mOutputControl.setLeft(mInputControl.getLeft());
        else if (mInputControl.getRight() > 0)
          mOutputControl.setRight(mInputControl.getRight());
        else
          mOutputControl.resetDirection();

        mPreviousPower = mOutputControl.getPower();
        mPreviousBrake = mOutputControl.getBrake();
      //  Logger::Instance().write(LogLevel::DEBUG, FPSTR("Exponential_power_modulation:Power (mOutputControl)"), String(mOutputControl.getPower()) + " " + String(mOutputControl.getBrake()));
        //Logger::Instance().write(LogLevel::DEBUG, FPSTR("Exponential_power_modulation:Turning (mOutputControl)"), String(mOutputControl.getLeft()) + " " + String(mOutputControl.getRight()));
      }
      else
      {
        mInputControl.reset();
        mOutputControl.reset();
      }
    }
  }
}

byte Exponential_power_modulation::calculateOutput(byte previousPower, byte power)
{
    //Smooth the input
    float targetAlpha = (mSmoothAlpha * (float)power) + ((1 - mSmoothAlpha) * (float)previousPower);
    //If the value is close to target, set it to target
    if (abs(float(power) - float(targetAlpha)) <= 1)
      targetAlpha = power;
    return (byte)targetAlpha;
}

void Exponential_power_modulation::command(byte command)
{

}

void Exponential_power_modulation::onDisable()
{
  mInputControl.reset();
  mOutputControl.reset();
}
