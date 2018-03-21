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
          mOutputControl.setPower(calculateOutput(mPreviousPower, mInputControl.getPower(), mInputControl.getPowerMax()));
        else if (mInputControl.getBrake() > 0)
          mOutputControl.setBrake(calculateOutput(mPreviousBrake, mInputControl.getBrake(), mInputControl.getBrakeMax()));
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

byte Exponential_power_modulation::calculateOutput(byte previousPower, byte power, byte maxPower)
{
    //Smooth the input
    float targetAlpha = float(power);
    if (power > previousPower)
      targetAlpha = (mCfg -> smoothAlphaPositive * (float)power) + ((1.0 - mCfg -> smoothAlphaPositive) * (float)previousPower);
    else if (power < previousPower)
      targetAlpha = (mCfg -> smoothAlphaNegative * (float)power) + ((1.0 - mCfg -> smoothAlphaNegative) * (float)previousPower);
    //If the value is close to target, set it to target
    if ((float(power) - targetAlpha) < 1.0)
      targetAlpha = power;
    return constrain((byte)targetAlpha, 0, maxPower);
}

void Exponential_power_modulation::command(byte command)
{

}

void Exponential_power_modulation::onDisable()
{
  mInputControl.reset();
  mOutputControl.reset();
}
