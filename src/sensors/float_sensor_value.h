#ifndef FLOAT_SENSOR_VALUE_H
#define FLOAT_SENSOR_VALUE_H

#include <Arduino.h>
#include "../interfaces/interfaces.hpp"

class Float_sensor_value : public ISensorvalue{
private:
  String mName = "";
  float mValue = 0;
  bool mValueChanged = true;
protected:
public:
    Float_sensor_value(const String& name, float value) {
      mName = name;
      mValue = value;
    }

    String getName()
    {
      return mName;
    }

    float getValue() const
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
