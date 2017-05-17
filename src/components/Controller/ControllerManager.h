#ifndef ControllerManager_h
#define ControllerManager_h
#include "AbstractController.h"
#include "../Connection/Wifi.h"
#include "../ConfigController/ConfigController.h"
#include "../MotorController/MotorController.h"

class ControllerManager
{
public:
  // Some sort of abstract controller.
  // TODO:: read this https://en.wikibooks.org/wiki/C%2B%2B_Programming/Classes/Abstract_Classes
  AbstractController*  activeController;
  AbstractController*  availableControllers[5];  // Not sure if I should store the entire object or just the pointers.

  Wifi* wifi;
  ConfigController* configController;
  MotorController*  motorController;

  void allocateRegisteredController(AbstractController* controller);
  byte getControllerIndexById(byte id);

  ControllerManager(ConfigController* configController, MotorController* motorController, Wifi* wifi);
  void registerController(byte type, byte id); // create a new controller based on the type [mobile, nunchuck]
  void removeRegisteredController(AbstractController* controller); // destroys a certain controller.

  // Manage Active Controllers
  bool setActiveController(byte id); // set one of the controllers as active based on the id of the controller
  bool unsetActiveController();

  void listenToController();

};


#endif
