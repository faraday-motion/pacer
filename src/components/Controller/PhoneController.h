#ifndef PhoneController_h
#define PhoneController_h

#include "AbstractController.h"
#include "components/Connection/Wifi.h"


//TODO:: PhoneController.h should be an instance for each phoneController connected to the LEV
class PhoneController : public AbstractController
{
private:
  // Functiones for reading the wifi packets.
  byte getChecksum(byte* array, byte arraySize);
  bool validateChecksum(byte* array, byte arraySize);
  bool controllerIsActive;   // tells us if this instance of PhoneController is active

public:
  Wifi* wifi;
  PhoneController(ConfigController* configController, MotorController* motorController, Wifi* wifi, byte controllerType, byte controllerId);
  void read();
  bool registerAsActive(String controllerUUID);

};

#endif
