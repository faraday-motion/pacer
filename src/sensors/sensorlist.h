#ifndef SENSORLIST_H
#define SENSORLIST_H

#include <Arduino.h>
#include <vector>
#include "../log/logger.h"
#include "../interfaces/interfaces.hpp"
#include "./sensors.hpp"

class SensorList : public ISensorList {
private:
  std::vector<ISensorValue*> mAllVector;
  std::vector<Bool_sensor_value*> mBoolVector;
  std::vector<Float_sensor_value*> mFloatVector;
  std::vector<Int_sensor_value*> mIntVector;
  std::vector<Byte_sensor_value*> mByteVector;
  std::vector<String_sensor_value*> mStringVector;
public:
  SensorList() {

  };

  std::vector<ISensorValue*> all() const
  {
    return mAllVector;
  }

  ISensorValue * getSensor(const String& name)
  {
    if (name == "")
    {
      Logger::Instance().write(LogLevel::ERROR, FPSTR("Name cannot be blank: "), "getSensor");
      return nullptr;
    }
    for (int i=0; i<mAllVector.size(); i++)
    {
      if (mAllVector[i] -> getName() == name)
        return mAllVector[i];
    }
    return nullptr;
  }

  Bool_sensor_value * getBoolSensor(const String& name)
  {
    if (name == "")
    {
      Logger::Instance().write(LogLevel::ERROR, FPSTR("Name cannot be blank: "), "getBoolSensor");
      return nullptr;
    }
    for (int i=0; i<mBoolVector.size(); i++)
    {
      if (mBoolVector[i] -> getName() == name)
        return mBoolVector[i];
    }
    return nullptr;
  }

  Float_sensor_value * getFloatSensor(const String& name)
  {
    if (name == "")
    {
      Logger::Instance().write(LogLevel::ERROR, FPSTR("Name cannot be blank: "), "getFloatSensor");
      return nullptr;
    }
    for (int i=0; i<mFloatVector.size(); i++)
    {
      if (mFloatVector[i] -> getName() == name)
        return mFloatVector[i];
    }
    return nullptr;
  }

  Int_sensor_value * getIntSensor(const String& name)
  {
    if (name == "")
    {
      Logger::Instance().write(LogLevel::ERROR, FPSTR("Name cannot be blank: "), "getIntSensor");
      return nullptr;
    }
    for (int i=0; i<mIntVector.size(); i++)
    {
      if (mIntVector[i] -> getName() == name)
        return mIntVector[i];
    }
    return nullptr;
  }

  Byte_sensor_value * getByteSensor(const String& name)
  {
    if (name == "")
    {
      Logger::Instance().write(LogLevel::ERROR, FPSTR("Name cannot be blank: "), "getByteSensor");
      return nullptr;
    }
    for (int i=0; i<mByteVector.size(); i++)
    {
      if (mByteVector[i] -> getName() == name)
        return mByteVector[i];
    }
    return nullptr;
  }

  String_sensor_value * getStringSensor(const String& name)
  {
    if (name == "")
    {
      Logger::Instance().write(LogLevel::ERROR, FPSTR("Name cannot be blank: "), "getStringSensor");
      return nullptr;
    }
    for (int i=0; i<mStringVector.size(); i++)
    {
      if (mStringVector[i] -> getName() == name)
        return mStringVector[i];
    }
    return nullptr;
  }

  Bool_sensor_value * setBoolSensor(const String& name, bool value)
  {
    if (name == "")
    {
      Logger::Instance().write(LogLevel::ERROR, FPSTR("Name cannot be blank: "), "setBoolSensor");
      return nullptr;
    }
    Bool_sensor_value * sv = getBoolSensor(name);
    if (sv == nullptr)
    {
      sv = new Bool_sensor_value(name, value);
      mAllVector.push_back(sv);
      mBoolVector.push_back(sv);
    }
    else
      sv -> setValue(value);
    return sv;
  }

  Float_sensor_value * setFloatSensor(const String& name, float value)
  {
    if (name == "")
    {
      Logger::Instance().write(LogLevel::ERROR, FPSTR("Name cannot be blank: "), "setFloatSensor");
      return nullptr;
    }
    Float_sensor_value * sv = getFloatSensor(name);
    if (sv == nullptr)
    {
      sv = new Float_sensor_value(name, value);
      mAllVector.push_back(sv);
      mFloatVector.push_back(sv);
    }
    else
      sv -> setValue(value);
    return sv;
  }

  Int_sensor_value * setIntSensor(const String& name, int value)
  {
    if (name == "")
    {
      Logger::Instance().write(LogLevel::ERROR, FPSTR("Name cannot be blank: "), "setIntSensor");
      return nullptr;
    }
    Int_sensor_value * sv = getIntSensor(name);
    if (sv == nullptr)
    {
      sv = new Int_sensor_value(name, value);
      mAllVector.push_back(sv);
      mIntVector.push_back(sv);
    }
    else
      sv -> setValue(value);
    return sv;
  }

  Byte_sensor_value * setByteSensor(const String& name, byte value)
  {
    if (name == "")
    {
      Logger::Instance().write(LogLevel::ERROR, FPSTR("Name cannot be blank: "), "setByteSensor");
      return nullptr;
    }
    Byte_sensor_value * sv = getByteSensor(name);
    if (sv == nullptr)
    {
      sv = new Byte_sensor_value(name, value);
      mAllVector.push_back(sv);
      mByteVector.push_back(sv);
    }
    else
      sv -> setValue(value);
    return sv;
  }

  String_sensor_value * setStringSensor(const String& name, String value)
  {
    if (name == "")
    {
      Logger::Instance().write(LogLevel::ERROR, FPSTR("Name cannot be blank: "), "setStringSensor");
      return nullptr;
    }
    String_sensor_value * sv = getStringSensor(name);
    if (sv == nullptr)
    {
      sv = new String_sensor_value(name, value);
      mAllVector.push_back(sv);
      mStringVector.push_back(sv);
    }
    else
      sv -> setValue(value);
    return sv;
  }
};
#endif
