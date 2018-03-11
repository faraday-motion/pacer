#include "./exponential_power_modulation.h"
#include "../base/base.hpp"
#include <Arduino.h>
#include "../../utility/simpletimer.h"
#include "../../configuration/default/configuration.h"

void Exponential_power_modulation::setup() {
  if (mIsSetup == false)
  {
    mIsSetup = true;
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up "), getModuleName());
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up "), getModuleName());
  }
}

void Exponential_power_modulation::loop()
{
  if (enabled())
  {
    if (mSimpleTimer.check())
    {
      Logger::Instance().write(LogLevel::DEBUG, getModuleName(), FPSTR("::loop"));
      IControlModule * mb = mFMV -> modules().getActiveControl();
      if (mb != nullptr)
      {
        IControlModule * ib = static_cast<IControlModule*>(mb);
        if (ib == nullptr)
          return;
        Control_module * ic = static_cast<Control_module*>(ib);
        if (ic == nullptr)
          return;
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
    //TODO Need to rework this calculation as it does not go to max when smoothalpha is less than 0,5
    //Smooth the input
    float targetAlpha = (mSmoothAlpha * (float)power) + ((1 - mSmoothAlpha) * (float)previousPower);
    //If the value is close to target, set it to target
    if (abs(float(power) - float(targetAlpha)) <= 1)
      targetAlpha = power;
    targetAlpha = constrain(targetAlpha, 0, 100);
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
