#ifndef SENSORBASE_H
#define SENSORBASE_H

#include <Arduino.h>
#include "../../enums/sensors.h"
#include "../../utility/simpletimer.h"

class Sensorbase {
private:
  float mValue = -1;
  bool mEnabled = true;
  bool mValueChanged = true;
  bool mHasNewValue = true;
  Sensors mSensor = Sensors::NONE;
protected:
  SimpleTimer mSimpleTimer;
  bool mIsSetup = false;
  byte mId = 0;
  Sensorbase(byte id, Sensors sensor){
    mId = id;
    mSensor = sensor;
  }
public:
    virtual void setup() = 0;
    virtual void loop() = 0;

    void setValue(float value)
    {
      mHasNewValue = true;
      if (mValue != value)
      {
        mValue = value;
        mValueChanged = true;
      }
      else
        mValueChanged = false;
    }

    bool valueChanged()
    {
      return mValueChanged;
    }

    bool hasNewValue()
    {
      return mHasNewValue;
    }

    unsigned long getInterval()
    {
        return mSimpleTimer.getInterval();
    }

    unsigned long getCritical()
    {
        return mSimpleTimer.getCritical();
    }

    void enable()
    {
      mEnabled = true;
    }

    void disable()
    {
      mEnabled = false;
    }

    bool enabled()
    {
      return mEnabled;
    }

    byte id()
    {
      return mId;
    }

    float value()
    {
      mHasNewValue = false;
      return mValue;
    }

    Sensors sensor()
    {
      return mSensor;
    }
};

#endif
