#ifndef AbstractDevice_H
#define AbstractDevice_H
#include <Arduino.h>
#include "components/ConfigController/ConfigStructs.h"

class AbstractDevice {

public:
  AbstractDevice();
  AbstractDevice(_wiredDevice device);
  byte id[5];
  byte type;
  byte priority;
  bool enabled;
  int  accelConstraint;
  int  brakeConstraint;
  byte pending = false;
  byte address[5];
  byte channel = 100;

};

#endif
