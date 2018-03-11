#ifndef STRING_SENSOR_VALUE_H
#define STRING_SENSOR_VALUE_H

#include <Arduino.h>
#include "../interfaces/interfaces.hpp"

class String_sensor_value : public ISensorValue{
private:
  String mName = "";
  String mValue = "";
  bool mValueChanged = true;
protected:
public:
    String_sensor_value(const String& name, String value) {
      mName = name;
      mValue = value;
    }

    String getName()
    {
      return mName;
    }

    String getValue() const
    {
      return mValue;
    }

    bool valueChanged() const
    {
      return mValueChanged;
    }

    void setValue(String value)
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
      return mValue;
    }

    void resetValue()
    {
      mValue = "";
    }
};

#endif
