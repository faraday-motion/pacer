#include "./direct_power_modulation.h"
#include "../base/base.hpp"
#include <Arduino.h>
#include "../../utility/simpletimer.h"
#include "../../configuration/default/configuration.h"

void Direct_power_modulation::setup() {
  if (mIsSetup == false)
  {
    mIsSetup = true;
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up "), getModuleName());
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up "), getModuleName());
  }
}

void Direct_power_modulation::loop()
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
          mOutputControl.setPower(mInputControl.getPower());
        else if (mInputControl.getBrake() > 0)
          mOutputControl.setBrake(mInputControl.getBrake());
        else
          mOutputControl.resetPower();

        if (mInputControl.getLeft() > 0)
          mOutputControl.setLeft(mInputControl.getLeft());
        else if (mInputControl.getRight() > 0)
          mOutputControl.setRight(mInputControl.getRight());
        else
          mOutputControl.resetDirection();
      }
      else
      {
        mInputControl.reset();
        mOutputControl.reset();
      }
    }
  }
}


void Direct_power_modulation::command(byte command)
{

}

void Direct_power_modulation::onDisable()
{
  mInputControl.reset();
  mOutputControl.reset();
}
