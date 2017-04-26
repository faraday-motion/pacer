#ifndef PhoneController_h
#define PhoneController_h

#include "Controller.h"
#include "components/Connection/Wifi.h"

class PhoneController
{
private:
  // Functiones for reading the wifi packets.
  byte getChecksum(byte* array, byte arraySize);
  bool validateChecksum(byte* array, byte arraySize);

public:
  Controller* controller;
  Wifi* wifi;
  PhoneController();
  void setup(Controller* baseController, Wifi* wifi);
  void read();

};

#endif
