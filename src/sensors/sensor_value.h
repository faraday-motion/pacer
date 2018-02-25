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
  bool mBoolValue = 0;
  bool hasBoolValue = false;
  bool mValueChanged = true;
  bool mHasNewValue = true;
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

    Sensor_value(String name, bool value) {
      mName = name;
      mBoolValue = value;
      hasBoolValue = true;
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

    int getBoolValue()
    {
      return mBoolValue;
    }

    bool valueChanged()
    {
      return mValueChanged;
    }

    bool hasNewValue()
    {
      return mHasNewValue;
    }

    void setValue(float value)
    {
       if (!hasFloatValue)
         resetValues();
       mHasNewValue = true;
       if (mFloatValue != value)
       {
         mFloatValue = value;
         mValueChanged = true;
       }
       else
         mValueChanged = false;
    }

    void setValue(String value)
    {
       if (!hasStringValue)
         resetValues();
       mHasNewValue = true;
       if (mStringValue != value)
       {
         mStringValue = value;
         mValueChanged = true;
       }
       else
         mValueChanged = false;
    }

    void setValue(int value)
    {
       if (!hasIntValue)
         resetValues();
       mHasNewValue = true;
       if (mIntValue != value)
       {
         mIntValue = value;
         mValueChanged = true;
       }
       else
         mValueChanged = false;
    }

    void setValue(bool value)
    {
       if (!hasBoolValue)
         resetValues();
       mHasNewValue = true;
       if (mBoolValue != value)
       {
         mBoolValue = value;
         mValueChanged = true;
       }
       else
         mValueChanged = false;
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
      else if (hasBoolValue)
      {
        return String(mBoolValue);
      }
    }

    void resetValues()
    {
      hasFloatValue = false;
      hasStringValue = false;
      hasIntValue = false;
      hasBoolValue = false;
      mFloatValue = 0;
      mStringValue = "";
      mIntValue = 0;
      mBoolValue = false;
    }

};

#endif
