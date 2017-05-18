#include "ControllerManager.h"
#include "AbstractController.h"
#include "PhoneController/PhoneController.h"
#include "NunchuckController/NunchuckController.h"

ControllerManager::ControllerManager(ConfigController* configController, Wifi* wifi)
{
  // TODO:: It seems a bit redundant to pass all these pointers to the controllerManager and then to the AbstractController. Investigate on how to optimize this.
  this->configController = configController;
  this->wifi             = wifi;

  activeController = nullptr;
  controllerReadInterval = new Metro(50);
  for (byte i = 0; i < 5; i++) {
    availableControllers[i] = nullptr;
  }
}

void ControllerManager::handleController()
{
  if (activeController != nullptr && controllerReadInterval->check() == 1){
    activeController->handleController();
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
  delete activeController; // Delete the object, hopefully... //TODO:: It might be wrong to delete the object when unsetting the as active. We lose the registered Controller.
  activeController = nullptr;
  return true;
}

void ControllerManager::registerController(byte controllerType, byte controllerId)
{
if (controllerType == 1) {
    Serial.println("Registering a PhoneController");
    AbstractController * phoneController = new PhoneController(configController, wifi, controllerType, controllerId);
    phoneController->setup();
    allocateRegisteredController(phoneController);
  } else if (controllerType == 2) {
    Serial.println("Registering a NunchuckController");
    AbstractController * nunchuckController = new NunchuckController(configController, controllerType, controllerId);
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

bool ControllerManager::allocateRegisteredController(AbstractController* controller)
{
  //TODO:: Handle case when no slots available.
  for (byte i = 0; i < 5; i++) {
    if(availableControllers[i] == nullptr)  {
      availableControllers[i] = controller;
      return true; // found free slot. Returning true.
    }
  }
  // No free slots where found
  return false;
}
