#include "ControllerManager.h"
#include "AbstractController.h"
#include "PhoneController/PhoneController.h"
#include "NunchuckController/NunchuckController.h"

ControllerManager::ControllerManager(ConfigController* configController, ConnectionManager* connectionManager)
{
  // TODO:: It seems a bit redundant to pass all these pointers to the controllerManager and then to the AbstractController. Investigate on how to optimize this.
  this->configController  = configController;
  this->connectionManager = connectionManager;

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

bool ControllerManager::setActiveController(byte id[])
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

bool ControllerManager::registerController(byte controllerType, byte controllerId[])
{
  if (this->getControllerIndexById(controllerId) != -1)
  {
    Serial.println("Controller Is Already Registered");
    return false;
  }

  if (controllerType == 1)
  {
      Serial.println("Registering a PhoneController");
      AbstractController * phoneController = new PhoneController(configController, connectionManager->wifi, controllerType, controllerId);
      allocateRegisteredController(phoneController);
      return true;
  }
  else if (controllerType == 2)
  {
      Serial.println("Registering a NunchuckController");
      AbstractController * nunchuckController = new NunchuckController(configController, connectionManager->radio, controllerType, controllerId);
      allocateRegisteredController(nunchuckController);
      return true;
  }
  else
  {
      Serial.println("Unknown type of controller");
      return false;
  }
}

int ControllerManager::getControllerIndexById(byte id[])
{
  for (byte i = 0; i < 5; i++) {
    if(availableControllers[i] != nullptr){  // not a null pointer
      if(availableControllers[i]->controllerId[0] == id[0] && availableControllers[i]->controllerId[1] == id[1] && availableControllers[i]->controllerId[2] == id[2] && availableControllers[i]->controllerId[3] == id[3] && availableControllers[i]->controllerId[4] == id[4] && availableControllers[i]->controllerId[5] == id[5]  ) {
        return i;
      }
    }
  }
  return -1;
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
