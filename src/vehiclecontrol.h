#ifndef VEHICLECONTROL_H
#define VEHICLECONTROL_H

#include <Arduino.h>
#include "./logs/logger.h"

class Vehiclecontrol {
private:
  static const byte mMaxmax = 100;
  byte mPower = 0;
  byte mBrake = 0;
  byte mLeft = 0;
  byte mRight = 0;
  byte mPowerMax = mMaxmax;
  byte mBrakeMax = mMaxmax;
  byte mLeftMax = mMaxmax;
  byte mRightMax = mMaxmax;
  String mName = "";
public:
  Vehiclecontrol() {

  };

  Vehiclecontrol(String name) {
    mName = name;
  };

  Vehiclecontrol(const Vehiclecontrol &vc) {
    mPowerMax = byte(vc.getPowerMax());
    mBrakeMax = byte(vc.getBrakeMax());
    mLeftMax = byte(vc.getLeftMax());
    mRightMax = byte(vc.getRightMax());
    mPower = byte(vc.getPower());
    mBrake = byte(vc.getBrake());
    mLeft = byte(vc.getLeft());
    mRight = byte(vc.getRight());
  };

  void setName(String name) {
    mName = name;
  }

  byte getPower() const
  {
    return mPower;
  }

  byte getBrake() const
  {
    return mBrake;
  }

  byte getPowerMax() const
  {
    return mPowerMax;
  }

  byte getBrakeMax() const
  {
    return mBrakeMax;
  }

  byte getLeft() const
  {
    return mLeft;
  }

  byte getRight() const
  {
    return mRight;
  }

  byte getLeftMax() const
  {
    return mLeftMax;
  }

  byte getRightMax() const
  {
    return mRightMax;
  }

  void setPowerMax(byte power)
  {
    mPowerMax = constrain(power, 0, mMaxmax);
  }

  void setBrakeMax(byte brake)
  {
    mBrakeMax = constrain(brake, 0, mMaxmax);
  }

  void setLeftMax(byte left)
  {
    mLeftMax = constrain(left, 0, mMaxmax);
  }

  void setRightMax(byte right)
  {
    mRightMax = constrain(right, 0, mMaxmax);
  }

  void setPower(byte power)
  {
    power = constrain(power, 0, mPowerMax);
    if (power > 0)
      mBrake = 0;
    mPower = power;
  }

  void setBrake(byte brake)
  {
    brake = constrain(brake, 0, mBrakeMax);
    if (brake > 0)
      mPower = 0;
    mBrake = brake;
  }

  void setLeft(byte left)
  {
    left = constrain(left, 0, mLeftMax);
    if (left > 0)
      mRight = 0;
    mLeft = left;
  }

  void setRight(byte right)
  {
    right = constrain(right, 0, mRightMax);
    if (right > 0)
      mLeft = 0;
    mRight = right;
  }

  void reset()
  {
    resetPower();
    resetDirection();
  }

  void resetPower()
  {
    mPower = 0;
    mBrake = 0;
  }

  void resetDirection()
  {
    mLeft = 0;
    mRight = 0;
  }

};
#endif
