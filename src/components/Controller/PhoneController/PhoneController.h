#ifndef PhoneController_h
#define PhoneController_h

#include "../AbstractController.h"
#include "components/Connection/Wifi.h"
#include <Metro.h>


//TODO:: PhoneController.h should be an instance for each phoneController connected to the LEV
class PhoneController : public AbstractController
{
private:

  /**
    Intervals
  */
  unsigned int _LOST_CONNECTION = 500;
  /**
    Identification
  */
  RadioDevice phone;

  /**
    Metro Timers
  */
  Metro* connectionLostTimer;
  // Functiones for reading the wifi packets.
  byte getChecksum(byte* array, byte arraySize);
  bool validateChecksum(byte* array, byte arraySize);
  bool controllerIsActive;   // tells us if this instance of PhoneController is active // not sure if we will use this.



public:
  Wifi* wifi;
  PhoneController(ConfigController* configController, Wifi* wifi, RadioDevice device);
  void read();
  void write();
  bool enable();
  bool handleController();

};

#endif
