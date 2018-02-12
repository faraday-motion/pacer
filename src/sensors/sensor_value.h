#ifndef SENSOR_VALUE_H
#define SENSOR_VALUE_H

#include <Arduino.h>

class Sensor_value {
private:
  String mName = "";
  float mFloatValue = 0;
  bool hasFloatValue = false;
  String mStringValue = "";
  bool hasStringValue = false;
  int mIntValue = 0;
  bool hasIntValue = false;
protected:
public:
    Sensor_value(String name, float value) {
      mName = name;
      mFloatValue = value;
      hasFloatValue = true;
    }

    Sensor_value(String name, String value) {
      mName = name;
      mStringValue = value;
      hasStringValue = true;
    }

    Sensor_value(String name, int value) {
      mName = name;
      mIntValue = value;
      hasIntValue = true;
    }

    String getName()
    {
      return mName;
    }

    float getFloatValue()
    {
      return mFloatValue;
    }

    String getStringValue()
    {
      return mStringValue;
    }

    int getIntValue()
    {
      return mIntValue;
    }

    void setValue(float value)
    {
       if (!hasFloatValue)
         resetValues();
       mFloatValue = value;
    }

    void setValue(String value)
    {
       if (!hasStringValue)
         resetValues();
       mStringValue = value;
    }

    void setValue(int value)
    {
       if (!hasIntValue)
         resetValues();
       mIntValue = value;
    }

    String getValue()
    {
      if (hasFloatValue)
      {
        return String(mFloatValue);
      }
      else if (hasStringValue)
      {
        return mStringValue;
      }
      else if (hasIntValue)
      {
        return String(mIntValue);
      }
    }

    void resetValues()
    {
      hasFloatValue = false;
      hasStringValue = false;
      hasIntValue = false;
      mFloatValue = 0;
      mStringValue = "";
      mIntValue = 0;
    }

};

#endif
