#ifndef RadioDevice_H
#define RadioDevice_H
#include <Arduino.h>
#include "AbstractDevice.h"

class RadioDevice : public AbstractDevice {

public:
  // this should be concrete for a radio device.
  // byte address[5];
  // byte channel = 100;

};

#endif
