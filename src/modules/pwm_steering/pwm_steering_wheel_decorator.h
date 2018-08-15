#ifndef PWM_STEERING_WHEEL_DECORATOR_H
#define PWM_STEERING_WHEEL_DECORATOR_H

#include <Arduino.h>
#include <Servo.h>
#include "./vehiclecontrol.h"
#include "../../configuration/default/configuration.h"
#include "../../interfaces/interfaces.hpp"

class Pwm_steering_wheel_decorator{
private:
  Vehiclecontrol mWheelControl;
  IWheel * mWheel = nullptr;
  Servo * mServo = nullptr;
  byte mMinAngle;
  byte mNeutral;
  byte mMaxAngle;
public:
  Pwm_steering_wheel_decorator(IWheel * wheel, Servo * servo, byte minAngle, byte neutral, byte maxAngle) {
    mWheel = wheel;
    mServo = servo;
    mMinAngle = minAngle;
    mNeutral = neutral;
    mMaxAngle = maxAngle;
  }

  Vehiclecontrol getWheelControl() const
  {
    return mWheelControl;
  }

  void setWheelControl(Vehiclecontrol wheelControl)
  {
    mWheelControl = wheelControl;
    if (mServo != nullptr)
    {
      //Logger::Instance().write(LogLevel::DEBUG, "Pwm_steering_wheel_decorator::Turning " + String(mWheelControl.getLeft()) + " " + String(mWheelControl.getRight()));
      if (mWheelControl.getLeft() > 0)
        if (!mWheel -> isInverse())
          mServo -> write(map(mWheelControl.getLeft(), 0, mWheelControl.getLeftMax(), mNeutral-1, mMinAngle));
        else
          mServo -> write(map(mWheelControl.getLeft(), 0, mWheelControl.getLeftMax(), mNeutral+1, mMaxAngle));
      else if (mWheelControl.getRight() > 0)
        if (!mWheel -> isInverse())
          mServo -> write(map(mWheelControl.getRight(), 0, mWheelControl.getRightMax(), mNeutral+1, mMaxAngle));
        else
          mServo -> write(map(mWheelControl.getRight(), 0, mWheelControl.getRightMax(), mNeutral-1, mMinAngle));
      else
        mServo -> write(mNeutral);
    }
  }

  IWheel * getWheel() const
  {
    return mWheel;
  }

  Servo * getServo() const
  {
    return mServo;
  }
};

#endif
