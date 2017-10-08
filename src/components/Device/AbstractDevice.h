#ifndef AbstractDevice_H
#define AbstractDevice_H
#include <Arduino.h>
#include "components/Config/ConfigStructs.h"

class AbstractDevice {

public:
  AbstractDevice();
  AbstractDevice(_registeredController device);
  byte id[5];
  byte type;
  byte priority;
  bool enabled;
  int  accelConstraint;
  int  brakeConstraint;
  bool pending = false;
  bool isRegisterd = false;

  byte address[5];
  byte channel = 100;

  // Debug
  void print();

};

#endif
