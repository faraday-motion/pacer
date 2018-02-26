#ifndef INT_SENSOR_VALUE_H
#define INT_SENSOR_VALUE_H

#include <Arduino.h>
#include "../interfaces/interfaces.hpp"

class Int_sensor_value : public ISensorvalue {
private:
  String mName = "";
  int mValue = 0;
  bool mValueChanged = true;
protected:
public:
    Int_sensor_value(const String& name, int value) {
      mName = name;
      mValue = value;
    }

    String getName()
    {
      return mName;
    }

    int getValue() const
    {
      return mValue;
    }

    bool valueChanged() const
    {
      return mValueChanged;
    }

    void setValue(float value)
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
