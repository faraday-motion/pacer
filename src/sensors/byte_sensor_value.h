#ifndef BYTE_SENSOR_VALUE_H
#define BYTE_SENSOR_VALUE_H

#include <Arduino.h>
#include "../interfaces/interfaces.hpp"

class Byte_sensor_value : public ISensorValue {
private:
  String mName = "";
  byte mValue = 0;
  bool mValueChanged = true;
protected:
public:
    Byte_sensor_value(const String& name, byte value) {
      mName = name;
      mValue = value;
    }

    String getName()
    {
      return mName;
    }

    byte getValue() const
    {
      return mValue;
    }

    bool valueChanged() const
    {
      return mValueChanged;
    }

    void setValue(byte value)
    {
       if (mValue != value)
       {
         mValue = value;
         mValueChanged = true;
       }
       else
         mValueChanged = false;
    }

    String getStringValue()
    {
      return String(mValue);
    }

    void resetValue()
    {
      mValue = 0;
    }
};

#endif
