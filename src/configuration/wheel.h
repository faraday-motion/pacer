#ifndef WHEEL_H
#define WHEEL_H

#include <Arduino.h>
#include "./configurator.h"
#include "./configuration/wheel_config.h"
#include "../interfaces/interfaces.hpp"

class Wheel : public IWheel{
private:
  byte mId = 0;
  byte mIndex = 0;
  bool mIsElectric = false;
  bool mCanSteer = false;
  bool mIsInverse = false;
  Wheel_config* mCfg = nullptr;
public:
  Wheel(byte id) {
    mId = id;
    mCfg = static_cast<Wheel_config*>(Configurator::Instance().createConfig(id, Configurations::WHEEL_CONFIG));
    mIndex = mCfg -> index;
    mIsElectric = mCfg -> isElectric;
    mCanSteer = mCfg -> canSteer;
    mIsInverse = mCfg -> isInverse;
  };

  bool isElectric() const
  {
    return mIsElectric;
  }

  bool canSteer() const
  {
    return mCanSteer;
  }

  byte getIndex() const
  {
    return mIndex;
  }

  bool isInverse() const
  {
    return mIsInverse;
  }

};
#endif
