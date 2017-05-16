#include "ControllerManager.h"
#include "AbstractController.h"
#include "PhoneController/PhoneController.h"
#include "NunchuckController/NunchuckController.h"

ControllerManager::ControllerManager(ConfigController* configController, MotorController* motorController, Wifi* wifi)
{
  this->configController = configController;
  this->motorController  = motorController;
  this->wifi             = wifi;

  activeController = nullptr;

  for (byte i = 0; i < 5; i++) {
    availableControllers[i] = nullptr;
  }
}

void ControllerManager::listenToController()
{
  if (activeController != nullptr){
    activeController->listenToController();
    return;
  }
}

bool ControllerManager::setActiveController(byte id)
{
  byte index = getControllerIndexById(id);
  activeController = availableControllers[index];
  return true;
}

bool ControllerManager::unsetActiveController()
{
  delete activeController; // Delete the object, hopefully...
  activeController = nullptr;
  return true;
}

void ControllerManager::registerController(byte controllerType, byte controllerId)
{
if (controllerType == 1) {
    Serial.println("Registering a PhoneController");
    AbstractController * phoneController = new PhoneController(configController, motorController, wifi, controllerType, controllerId);
    phoneController->setup();
    allocateRegisteredController(phoneController);
  } else if (controllerType == 2) {
    Serial.println("Registering a NunchuckController");
    AbstractController * nunchuckController = new NunchuckController(configController, motorController, controllerType, controllerId);
    allocateRegisteredController(nunchuckController);
  } else {
    Serial.println("Unknown type of controller");
    return;
  }
}


byte ControllerManager::getControllerIndexById(byte id)
{
  for (byte i = 0; i < 5; i++) {
    if(availableControllers[i]->controllerId == id) {
      return i;
    }
  }
}

void ControllerManager::allocateRegisteredController(AbstractController* controller)
{
  //TODO:: Handle case when no slots available.
  for (byte i = 0; i < 5; i++) {
    if(availableControllers[i] == nullptr)  {
      availableControllers[i] = controller;
      break; // exit loop
    }
  }
}
