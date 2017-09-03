#ifndef ControllerManager_h
#define ControllerManager_h

#include <Arduino.h>
#include <Metro.h>
#include "AbstractController.h"
#include "components/Device/AbstractDevice.h"
#include "components/Connection/ConnectionManager.h"
#include "components/MotorController/MotorController.h"
#include "components/ConfigController/ConfigController.h"

class ControllerManager
{

private:
   Metro* controllerReadInterval;
   Metro* printInterval;
   ConnectionManager* connectionManager;

public:
  AbstractController*  activeController;
  AbstractController*  availableControllers[5];  // Not sure if I should store the entire object or just the pointers.
  byte registeredControllersCount = 0;
  ConfigController* configController;
  MotorController*  motorController;

  void loop();

  bool allocateRegisteredController(AbstractController* controller);
  int getControllerIndexById(byte id[]);

  ControllerManager(ConfigController* configController, ConnectionManager* connectionManager);

  bool registerController(AbstractDevice device); // create a new controller based on the type [mobile, nunchuck, accel, wired controller]
  void removeRegisteredController(byte id[]); // destroys a certain controller.

  // Manage Active Controllers
  AbstractController* getActiveControllerId();
  bool setActiveController(byte id[]); // set one of the controllers as active based on the id of the controller
  bool unsetActiveController();
  bool tryOtherControllers();
  byte handleActiveController();
  

  // Debug
  void printRegisteredControllers();
  void printActiveController();


};


#endif
