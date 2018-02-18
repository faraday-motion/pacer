#ifndef VESC_CONTROLLER_WHEEL_DECORATOR_H
#define VESC_CONTROLLER_WHEEL_DECORATOR_H

#include <Arduino.h>
#include "./vehiclecontrol.h"
#include "../../configuration/default/configuration.h"
#include "../../configuration/wheel.h"
#include "datatypes.h"

class Vesc_controller_wheel_decorator{
private:
  Vehiclecontrol mWheelControl;
  Wheel* mWheel = nullptr;
  mc_values mVescValues;
  bool mIsMaster = false;
public:
  Vesc_controller_wheel_decorator(Wheel* wheel) {
    mWheel = wheel;
  }

  Vehiclecontrol getWheelControl() const
  {
    return mWheelControl;
  }

  void setWheelControl(Vehiclecontrol wheelControl)
  {
    mWheelControl = wheelControl;
  }

  int getCanId() const
  {
    if (mWheel -> getIndex() == 0)
      return -1;
    else
      return mWheel -> getIndex();
  }

  Wheel* getWheel() const
  {
    return mWheel;
  }

  mc_values getVescValues() const
  {
    return mVescValues;
  }

  void setVescValues(mc_values vescValues)
  {
    mVescValues = vescValues;
    
  }

  bool isMaster()
  {
    return mIsMaster;
  }

  void setMaster(bool master)
  {
    mIsMaster = master;
  }
};

#endif
