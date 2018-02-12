/*
#include "./direct_power_modulation.h"
#include <Arduino.h>
#include "../../utility/simpletimer.h"
#include "../../configuration/default/configuration.h"
#include "../base/input_control.h"

void Direct_power_modulation::setup() {
  if (mIsSetup == false)
  {
    mIsSetup = true;
  }
}

void Direct_power_modulation::loop()
{
  if (enabled())
  {
    if (mSimpleTimer.check()) {
      Modulebase* mb = mFMV -> getActiveControl();
      if (mb != nullptr)
      {
        mOutputControl.setPower(mInputControl.getPower());
        mOutputControl.setBrake(mInputControl.getBrake());
        mOutputControl.setLeft(mInputControl.getLeft());
        mOutputControl.setRight(mInputControl.getRight());
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
*/
