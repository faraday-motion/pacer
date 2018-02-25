#ifndef CONTROL_MODULE_H
#define CONTROL_MODULE_H
#include <Arduino.h>
#include "./base.hpp"
#include "../../configuration/default/configuration.h"
#include "../../vehiclecontrol.h"
#include "../../log/logger.h"
#include "../../utility/simpletimer.h"

class Control_module : public Modulebase, public IRecieve, public IActive, public IClient  {
private:
  bool mIsActive = false;
  bool mHasClient = false;
  SimpleTimer mSimpleTimerClientTimeout;
protected:
  Vehiclecontrol mOutputControl;
  Control_module(byte id, int module) : Modulebase(id, module, Roles::CONTROL_MODULE) {
    mSimpleTimerClientTimeout.setInterval(30000);
  }

  void setClientTimeout(String name, int timeout)
  {
    mOutputControl.setName(name);
    mSimpleTimerClientTimeout.setName(name);
    mSimpleTimerClientTimeout.setInterval(timeout);
  }

  void setHasClient(bool hasClient)
  {
    if (hasClient)
    {
      //Logger::Instance().write(LogLevel::DEBUG, "setHasClient true");
      mSimpleTimerClientTimeout.reset();
    }
    else
    {
      //Logger::Instance().write(LogLevel::DEBUG, "setHasClient.reset()");
      mOutputControl.reset();
      onEvent(Events::CONTROL_NONE);
    }
    mHasClient = hasClient;
  }

  void clientTimeoutCheck()
  {
    if (mSimpleTimerClientTimeout.check())
      setHasClient(false);
  }

  void recieve(byte command, byte value)
  {
    Logger::Instance().write(LogLevel::INFO, FPSTR("Control_module::recieve Command: "), String(command) + " Value: " + String(value));
    if (enabled())
    {
      Logger::Instance().write(LogLevel::INFO, FPSTR("Control_module::enabled true"));
      //If the command is not allowed to be executed
      if (command > EXTERNALCOMMANDS_LIMIT)
        return;
      //The commands that are allowed to be executed
      if (command == ExternalCommands::DRIVE_POWER)
      {
        Logger::Instance().write(LogLevel::INFO, FPSTR("Control_module::setPower:"), String(value));
        mOutputControl.setPower(value);
        if (value != 0)
          onEvent(Events::DRIVE_POWER);
        else
          onEvent(Events::DRIVE_NEUTRAL);
        setHasClient(true);
      }
      else if (command == ExternalCommands::DRIVE_BRAKE)
      {
        mOutputControl.setBrake(value);
        if (value != 0)
          onEvent(Events::DRIVE_BRAKE);
        else
          onEvent(Events::DRIVE_NEUTRAL);
        setHasClient(true);
      }
      else if (command == ExternalCommands::TURN_LEFT)
      {
        mOutputControl.setLeft(value);
        if (value != 0)
          onEvent(Events::TURN_LEFT);
        else
          onEvent(Events::TURN_NEUTRAL);
        setHasClient(true);
      }
      else if (command == ExternalCommands::TURN_RIGHT)
      {
        mOutputControl.setRight(value);
        if (value != 0)
          onEvent(Events::TURN_RIGHT);
        else
          onEvent(Events::TURN_NEUTRAL);
        setHasClient(true);
      }
      else if (command == ExternalCommands::ENABLE_CONTROLLER)
      {
        //mFMV -> modules().setActiveController(value);
      }
      else if(command == ExternalCommands::DISABLE_CONTROLLERS)
      {
        //mFMV -> modules().deactivateControllers();
      }
      else if(command == ExternalCommands::DRIVE_MODE_20)
      {
        onEvent(Events::DRIVE_MODE_20);
      }
      else if(command == ExternalCommands::DRIVE_MODE_40)
      {
        onEvent(Events::DRIVE_MODE_40);
      }
      else if(command == ExternalCommands::DRIVE_MODE_60)
      {
        onEvent(Events::DRIVE_MODE_60);
      }
      else if(command == ExternalCommands::DRIVE_MODE_80)
      {
        onEvent(Events::DRIVE_MODE_80);
      }
      else if(command == ExternalCommands::DRIVE_MODE_100)
      {
        onEvent(Events::DRIVE_MODE_100);
      }
    }
  }
public:
  bool isActive()
  {
    return mIsActive;
  }

  void setActive(bool active)
  {
    //Logger::Instance().write(LogLevel::DEBUG, "setActive.reset()");
    mOutputControl.reset();
    mIsActive = active;
  }

  bool hasClient()
  {
    return mHasClient;
  }

  enum Commands : byte {
  };

  enum Events : byte {
    CONFIGURE,
    MODULE_ENABLED,
    MODULE_DISABLED,
    CONTROL_NONE,
    TURN_NEUTRAL,
    TURN_LEFT,
    TURN_RIGHT,
    DRIVE_NEUTRAL,
    DRIVE_POWER,
    DRIVE_BRAKE,
    DRIVE_MODE_20,
    DRIVE_MODE_40,
    DRIVE_MODE_60,
    DRIVE_MODE_80,
    DRIVE_MODE_100
  };


  //Not sure if this actually does what I expect
  Vehiclecontrol getOutputControl() const
  {
    return mOutputControl ;
  }
};

#endif
