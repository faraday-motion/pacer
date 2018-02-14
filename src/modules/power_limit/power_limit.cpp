#include <Arduino.h>
#include "./power_limit.h"
#include "../../configuration/default/configuration.h"
#include "../base/modulation_module.h"

void Power_limit::setup() {
  if (mIsSetup == false)
  {
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up Power_limit"));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up Power_limit"));
    mIsSetup = true;
  }
}

void Power_limit::loop()
{
  if (enabled())
  {
    Logger::Instance().write(LogLevel::DEBUG, FPSTR("Power_limit::loop"));
    if (mIsVehicleDead)
    {
      mInputControl.reset();
      mOutputControl.reset();
    }
    else if (mSimpleTimer.check())
    {
      Modulebase* mb = mFMV -> modules().getEnabled(Roles::MODULATION_MODULE);
      if (mb != nullptr )
      {
        Modulation_module* ic = static_cast<Modulation_module*>(mb);
        mInputControl = Vehiclecontrol(ic -> getOutputControl());
        if (mInputControl.getPower() > 0)
        {
          if (mIsPowerReset)
          {
            mOutputControl.setPower(map(mInputControl.getPower(), 0, mInputControl.getPowerMax(), 0, mOutputControl.getPowerMax()));
          }
          else
            onEvent(Events::WARNING_SET_INPUT_NEUTRAL);
        }
        else if (mInputControl.getBrake() > 0)
        {
          if (mIsPowerReset)
          {
            mOutputControl.setBrake(map(mInputControl.getBrake(), 0, mInputControl.getBrakeMax(), 0, mOutputControl.getBrakeMax()));
          }
          else
            onEvent(Events::WARNING_SET_INPUT_NEUTRAL);
        }
        else
        {
          mIsPowerReset = true;
          mOutputControl.resetPower();
        }

        if (mInputControl.getLeft() > 0)
          mOutputControl.setLeft(map(mInputControl.getLeft(), 0, mInputControl.getLeftMax(), 0, mOutputControl.getLeftMax()));
        else if (mInputControl.getRight() > 0)
          mOutputControl.setRight(map(mInputControl.getRight(), 0, mInputControl.getRightMax(), 0, mOutputControl.getRightMax()));
        else
          mOutputControl.resetDirection();
      }
    }
    mFMV -> sensors().add("Power", mOutputControl.getPower());
    mFMV -> sensors().add("Brake", mOutputControl.getBrake());
    mFMV -> sensors().add("Left", mOutputControl.getLeft());
    mFMV -> sensors().add("Right", mOutputControl.getRight());
    //Logger::Instance().write(LogLevel::DEBUG, "Power_limit:Power " + String(mOutputControl.getPower()) + " " + String(mOutputControl.getBrake()));
    //Logger::Instance().write(LogLevel::DEBUG, "Power_limit:Turning " + String(mOutputControl.getLeft()) + " " + String(mOutputControl.getRight()));
  }
}

void Power_limit::command(byte command)
{
  if (enabled())
  {
    Commands comm = static_cast<Commands>(command);
    switch(comm) {
    case Commands::DRIVE_MODE_100 :
      setPowerMax(100);
      onEvent(Events::LIMIT_NONE);
      break;
    case Commands::DRIVE_MODE_80 :
      setPowerMax(80);
      onEvent(Events::LIMIT_80);
      break;
    case Commands::DRIVE_MODE_60 :
      setPowerMax(60);
      onEvent(Events::LIMIT_60);
      break;
    case Commands::DRIVE_MODE_40 :
      setPowerMax(40);
      onEvent(Events::LIMIT_40);
      break;
    case Commands::DRIVE_MODE_20 :
      setPowerMax(20);
      onEvent(Events::LIMIT_20);
      break;
    case Commands::DRIVE_MODE_0 :
      setPowerMax(0);
      onEvent(Events::LIMIT_0);
      break;
    case Commands::VEHICLE_DEAD :
      mIsVehicleDead = true;
      mIsPowerReset = false;
      mInputControl.reset();
      mOutputControl.reset();
      onEvent(Events::LIMIT_DEAD);
      break;
    case Commands::VEHICLE_ALIVE :
      mIsVehicleDead = false;
      onEvent(Events::LIMIT_ALIVE);
      break;
    }
  }
}

void Power_limit::setPowerMax(byte powermax)
{
  mOutputControl.setPowerMax(powermax);
  mOutputControl.setBrakeMax(powermax);
}

void Power_limit::onDisable()
{
    mIsVehicleDead = false;
    setPowerMax(100);
    mInputControl.reset();
    mOutputControl.reset();
    onEvent(Events::LIMIT_NONE);
}
