#ifndef CONTROL_MODULE_H
#define CONTROL_MODULE_H

#include <Arduino.h>
#include "../../configuration/configurator.h"
#include "./base.hpp"
#include "../../configuration/default/configuration.h"
#include "../../vehiclecontrol.h"
#include "../../log/logger.h"
#include "../../utility/simpletimer.h"
#include "../../interfaces/interfaces.hpp"

class Control_module : public Modulebase, public IControlModule {
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
    if (Modulebase::enabled())
    {
      if (Configurator::Instance().commandEnabled(command))
      {
        bool hasChange = false;
        if (command == ExternalCommands::DRIVE_POWER)
        {
          hasChange == mOutputControl.getPower() != value;
          mOutputControl.setPower(value);
          if (hasChange)
          {
            if (value != 0)
              onEvent(Events::DRIVE_POWER);
            else
              onEvent(Events::DRIVE_NEUTRAL);
          }
          setHasClient(true);
        }
        else if (command == ExternalCommands::DRIVE_BRAKE)
        {
          hasChange == mOutputControl.getBrake() != value;
          mOutputControl.setBrake(value);
          if (hasChange)
          {
            if (value != 0)
              onEvent(Events::DRIVE_BRAKE);
            else
              onEvent(Events::DRIVE_NEUTRAL);
          }
          setHasClient(true);
        }
        else if (command == ExternalCommands::TURN_LEFT)
        {
          hasChange == mOutputControl.getLeft() != value;
          mOutputControl.setLeft(value);
          if (hasChange)
          {
            if (value != 0)
              onEvent(Events::TURN_LEFT);
            else
              onEvent(Events::TURN_NEUTRAL);
          }
          setHasClient(true);
        }
        else if (command == ExternalCommands::TURN_RIGHT)
        {
          hasChange == mOutputControl.getRight() != value;
          mOutputControl.setRight(value);
          if (hasChange)
          {
            if (value != 0)
              onEvent(Events::TURN_RIGHT);
            else
              onEvent(Events::TURN_NEUTRAL);
          }
          setHasClient(true);
        }
        else if (command == ExternalCommands::LED_ENABLED)
        {
          onEvent(Events::LED_ENABLED);
        }
        else if(command == ExternalCommands::LED_DISABLED)
        {
          onEvent(Events::LED_DISABLED);
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
  }
public:
  bool isActive()
  {
    return mIsActive;
  }

  void setActive(bool active)
  {
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
    DRIVE_MODE_0,
    DRIVE_MODE_20,
    DRIVE_MODE_40,
    DRIVE_MODE_60,
    DRIVE_MODE_80,
    DRIVE_MODE_100,
    LED_ENABLED,
    LED_DISABLED
  };


  //Not sure if this actually does what I expect
  Vehiclecontrol getOutputControl() const
  {
    return mOutputControl ;
  }
};

#endif
