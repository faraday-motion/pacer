#ifndef BOOL_SENSOR_VALUE_H
#define BOOL_SENSOR_VALUE_H

#include <Arduino.h>
#include "../interfaces/interfaces.hpp"

class Bool_sensor_value : public ISensorValue {
private:
  String mName = "";
  bool mValue = false;
  bool mValueChanged = true;
protected:
public:
    Bool_sensor_value(const String& name, bool value) {
      mName = name;
      mValue = value;
    }

    String getName()
    {
      return mName;
    }

    bool getValue() const
    {
      return mValue;
    }

    bool valueChanged() const
    {
      return mValueChanged;
    }

    void setValue(bool value)
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
      mValue = false;
    }
};

#endif
