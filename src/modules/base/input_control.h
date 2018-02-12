#ifndef INPUT_CONTROL_H
#define INPUT_CONTROL_H
#include <Arduino.h>
#include "./modulebase.h"
#include "../../configuration/default/configuration.h"
#include "../../enums/enums.hpp"
#include "../../vehiclecontrol.h"
#include "../../fmv.h"
#include "../../logs/logger.h"
#include "../../utility/simpletimer.h"

class Input_control : public Modulebase  {
private:
  bool mIsActive = false;
  bool mHasClient = false;
  FMV* mFMV = nullptr;
  SimpleTimer mSimpleTimerClientTimeout;
protected:
  Vehiclecontrol mOutputControl;
  Input_control(byte id, Modules module, FMV* fmv) : Modulebase(id, module, Roles::INPUT_CONTROL) {
    mFMV = fmv;
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

  virtual void handleCommand(byte command, byte value)
  {
    handleCoreCommand(command, value);
  }

  void handleCoreCommand(byte command, byte value)
  {
    if (enabled())
    {
      //If the command is not allowed to be executed
      if (command > EXTERNALCOMMANDS_LIMIT)
        return;
      //The commands that are allowed to be executed
      if (command == (byte)ExternalCommands::DRIVE_POWER)
      {
        mOutputControl.setPower(value);
        if (value != 0)
          onEvent(Events::DRIVE_POWER);
        else
          onEvent(Events::DRIVE_NEUTRAL);
        setHasClient(true);
      }
      else if (command == (byte)ExternalCommands::DRIVE_BRAKE)
      {
        mOutputControl.setBrake(value);
        if (value != 0)
          onEvent(Events::DRIVE_BRAKE);
        else
          onEvent(Events::DRIVE_NEUTRAL);
        setHasClient(true);
      }
      else if (command == (byte)ExternalCommands::TURN_LEFT)
      {
        mOutputControl.setLeft(value);
        if (value != 0)
          onEvent(Events::TURN_LEFT);
        else
          onEvent(Events::TURN_NEUTRAL);
        setHasClient(true);
      }
      else if (command == (byte)ExternalCommands::TURN_RIGHT)
      {
        mOutputControl.setRight(value);
        if (value != 0)
          onEvent(Events::TURN_RIGHT);
        else
          onEvent(Events::TURN_NEUTRAL);
        setHasClient(true);
      }
      else if (command == (byte)ExternalCommands::ENABLE_CONTROLLER)
      {
        mFMV -> modules().setActiveController(value);
      }
      else if(command == (byte)ExternalCommands::DISABLE_CONTROLLERS)
      {
        mFMV -> modules().deactivateControllers();
      }
      else if(command == (byte)ExternalCommands::DRIVE_MODE_20)
      {
        onEvent(Events::DRIVE_MODE_20);
      }
      else if(command == (byte)ExternalCommands::DRIVE_MODE_40)
      {
        onEvent(Events::DRIVE_MODE_40);
      }
      else if(command == (byte)ExternalCommands::DRIVE_MODE_60)
      {
        onEvent(Events::DRIVE_MODE_60);
      }
      else if(command == (byte)ExternalCommands::DRIVE_MODE_80)
      {
        onEvent(Events::DRIVE_MODE_80);
      }
      else if(command == (byte)ExternalCommands::DRIVE_MODE_100)
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
