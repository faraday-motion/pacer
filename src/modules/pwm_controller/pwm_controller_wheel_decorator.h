#ifndef PWM_CONTROLLER_WHEEL_DECORATOR_H
#define PWM_CONTROLLER_WHEEL_DECORATOR_H

#include <Arduino.h>
#include <Servo.h>
#include "./vehiclecontrol.h"
#include "../../configuration/default/configuration.h"
#include "../../interfaces/interfaces.hpp"

class Pwm_controller_wheel_decorator{
private:
  Vehiclecontrol mWheelControl;
  IWheel * mWheel = nullptr;
  Servo * mServo = nullptr;
public:
  Pwm_controller_wheel_decorator(IWheel * wheel, Servo * servo) {
    mWheel = wheel;
    mServo = servo;
  }

  void setWheelControl(Vehiclecontrol wheelControl)
  {
    mWheelControl = wheelControl;
    if (mServo != nullptr)
    {
      //Logger::Instance().write(LogLevel::DEBUG, "Pwm_controller_wheel_decorator::Power " + String(mWheelControl.getPower()) + " " + String(mWheelControl.getBrake()));

      if (mWheelControl.getPower() > 0)
        mServo -> write(map(mWheelControl.getPower(), 0, 100, 91, 180));
      else if (mWheelControl.getBrake() > 0)
        mServo -> write(map(mWheelControl.getBrake(), 0, 100, 89, 0));
      else
        mServo -> write(90);
    }
  }

  Vehiclecontrol getWheelControl() const
  {
    return mWheelControl;
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
