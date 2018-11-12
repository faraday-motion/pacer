#include <Arduino.h>
#include "./power_limit.h"
#include "../../configuration/default/configuration.h"
#include "../base/modulation_module.h"

void Power_limit::setup() {
  if (mIsSetup == false)
  {
    mIsSetup = true;
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up "), getModuleName());
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    onEvent(Events::CONFIGURE);
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up "), getModuleName());
  }
}

void Power_limit::loop()
{
  if (enabled())
  {
    Logger::Instance().write(LogLevel::DEBUG, getModuleName(), FPSTR("::loop"));
    if (mSensor == nullptr && mCfg -> deadSensorName != "")
      mSensor = mFMV -> sensors().getBoolSensor(mCfg -> deadSensorName);

    bool isDead = mIsVehicleDead;
    if (mSensor != nullptr)
      isDead = mSensor -> getValue();

    //Only do on change
    if (isDead && mIsVehicleDead != isDead)
    {
      mIsPowerReset = false;
      onEvent(Events::LIMIT_DEAD);
    }
    else if (!isDead && mIsVehicleDead != isDead)
    {
      onEvent(Events::LIMIT_ALIVE);
    }
    mIsVehicleDead = isDead;

    if (!mIsVehicleDead)
    {
      mPauseSimpleTimer.reset();
      mIsPause = false;
    }

    if (mIsVehicleDead)
    {
      mInputControl.reset();
      mOutputControl.reset();
      if (mPauseSimpleTimer.check() && mIsPause == false)
      {
        mIsPause = true;
        onEvent(Events::LIMIT_DEAD_PAUSE);
      }
    }
    else if (mSimpleTimer.check())
    {
      IModule * mb = mFMV -> modules().getEnabledByRole(Roles::MODULATION_MODULE);
      if (mb != nullptr )
      {
        Logger::Instance().write(LogLevel::DEBUG, getModuleName(), FPSTR("::Modulation_module"));
        Modulation_module * ic = static_cast<Modulation_module*>(mb);
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
        Logger::Instance().write(LogLevel::INFO, FPSTR("Power_limit::accel: "), String(mOutputControl.getPower()));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Power_limit::brake: "), String(mOutputControl.getBrake()));
      }
    }
    mFMV -> sensors().setByteSensor("accel", mOutputControl.getPower());
    mFMV -> sensors().setByteSensor("brake", mOutputControl.getBrake());
    mFMV -> sensors().setByteSensor("left", mOutputControl.getLeft());
    mFMV -> sensors().setByteSensor("right", mOutputControl.getRight());
  }
}

void Power_limit::command(byte command)
{
  if (enabled())
  {
    Logger::Instance().write(LogLevel::INFO, FPSTR("Power_limit::command: "), String(command));
    Commands comm = static_cast<Commands>(command);
    switch(comm) {
    case Commands::DRIVE_MODE_100 :
      setPowerMax(100);
      onEvent(Events::LIMIT_100);
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
    onEvent(Events::LIMIT_100);
}
