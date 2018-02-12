#ifndef PWM_STEERING_WHEEL_DECORATOR_H
#define PWM_STEERING_WHEEL_DECORATOR_H

#include <Arduino.h>
#include <Servo.h>
#include "./vehiclecontrol.h"
#include "../../configuration/default/configuration.h"
#include "../../configuration/wheel.h"

class Pwm_steering_wheel_decorator{
private:
  Vehiclecontrol mWheelControl;
  Wheel* mWheel = nullptr;
  Servo* mServo = nullptr;
public:
  Pwm_steering_wheel_decorator(Wheel* wheel, Servo* servo) {
    mWheel = wheel;
    mServo = servo;
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
          mServo -> write(map(mWheelControl.getLeft(), 0, 100, 89, 0));
        else
          mServo -> write(map(mWheelControl.getLeft(), 0, 100, 91, 100));
      else if (mWheelControl.getRight() > 0)
        if (!mWheel -> isInverse())
          mServo -> write(map(mWheelControl.getRight(), 0, 100, 91, 180));
        else
          mServo -> write(map(mWheelControl.getRight(), 0, 100, 89, 0));
      else
        mServo -> write(90);
    }
  }

  Wheel* getWheel() const
  {
    return mWheel;
  }

  Servo* getServo() const
  {
    return mServo;
  }
};

#endif
