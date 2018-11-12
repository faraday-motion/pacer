#ifndef ISENSORLIST_H
#define ISENSORLIST_H

#include <vector>
#include <Arduino.h>
#include "./interfaces.hpp"
#include "../sensors/sensors.hpp"

class ISensorList
{
   public:
     virtual ISensorValue * getSensor(const String& name) = 0;
     virtual Int_sensor_value * setIntSensor(const String& name, int value) = 0;
     virtual Bool_sensor_value * setBoolSensor(const String& name, bool value) = 0;
     virtual Float_sensor_value * setFloatSensor(const String& name, float value) = 0;
     virtual Byte_sensor_value * setByteSensor(const String& name, byte value) = 0;
     virtual String_sensor_value * setStringSensor(const String& name, String value) = 0;
     virtual Bool_sensor_value * getBoolSensor(const String& name) = 0;
     virtual Float_sensor_value * getFloatSensor(const String& name) = 0;
     virtual Int_sensor_value * getIntSensor(const String& name) = 0;
     virtual Byte_sensor_value * getByteSensor(const String& name) = 0;
     virtual String_sensor_value * getStringSensor(const String& name) = 0;
     virtual std::vector<ISensorValue*> all() const = 0;
};
#endif
