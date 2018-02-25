#ifndef SENSORLIST_H
#define SENSORLIST_H

#include <Arduino.h>
#include <vector>
#include "./sensor_value.h"
#include "../log/logger.h"

class SensorList {
private:
  std::vector<Sensor_value*> sensorValueArray;
public:
  SensorList() {

  };

  std::vector<Sensor_value*> all() const
  {
    return sensorValueArray;
  }

  Sensor_value * get(String name)
  {
    for (int i=0; i<sensorValueArray.size(); i++)
    {
      if (sensorValueArray[i] -> getName() == name)
        return sensorValueArray[i];
    }
    return nullptr;
  }

  void add(String name, float value)
  {
    Sensor_value* sv = get(name);
    if (sv == nullptr)
      sensorValueArray.push_back(new Sensor_value(name, value));
    else
      sv -> setValue(value);
  }

  void add(String name, String value)
  {
    Sensor_value* sv = get(name);
    if (sv == nullptr)
      sensorValueArray.push_back(new Sensor_value(name, value));
    else
      sv -> setValue(value);
  }

  void add(String name, int value)
  {
    Sensor_value* sv = get(name);
    if (sv == nullptr)
      sensorValueArray.push_back(new Sensor_value(name, value));
    else
      sv -> setValue(value);
  }

  void add(String name, bool value)
  {
    Sensor_value* sv = get(name);
    if (sv == nullptr)
      sensorValueArray.push_back(new Sensor_value(name, value));
    else
      sv -> setValue(value);
  }

};
#endif
