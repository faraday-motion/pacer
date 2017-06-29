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
  controllerReadInterval = new Metro(10);
  printInterval = new Metro(500);

  for (byte i = 0; i < 5; i++)
  {
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

/**
 * Takes a registered controller id and sets it as active.
 * Calls the AbstractCotnroller->enable() method to let the controller receiver know it is active.
 */

bool ControllerManager::setActiveController(byte id[])
{
  byte index = getControllerIndexById(id);
  activeController = availableControllers[index];
  activeController->enable(); // Enable the activeController;
  return true;
}


bool ControllerManager::unsetActiveController()
{
  delete activeController; // Delete the object, hopefully... //TODO:: It might be wrong to delete the object when unsetting the as active. We lose the registered Controller.
  activeController = nullptr;
  return true;
}

bool ControllerManager::registerController(RadioDevice device)
{
  if (this->getControllerIndexById(device.id) != -1)
  {
    //Serial.println("Controller Is Already Registered");
    return false;
  }

  if (device.type == 1)
  {
      Serial.println("Registering a PhoneController");
      AbstractController * phoneController = new PhoneController(configController, connectionManager->wifi, device);
      allocateRegisteredController(phoneController);
      return true;
  }
  else if (device.type == 2)
  {
      Serial.println("Registering a NunchuckController");
      AbstractController * nunchuckController = new NunchuckController(configController, connectionManager->radio, device);
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
  //Serial.println("Checking if the ControllerID is already registered.");
  for (byte i = 0; i < 5; i++) {
    if(availableControllers[i] != nullptr){  // not a null pointer
      if(availableControllers[i]->controller.id[0] == id[0] && availableControllers[i]->controller.id[1] == id[1] && availableControllers[i]->controller.id[2] == id[2] && availableControllers[i]->controller.id[3] == id[3] && availableControllers[i]->controller.id[4] == id[4])
      {
        return i;
      }
    }
  }
  return -1;
}

bool ControllerManager::allocateRegisteredController(AbstractController* controller)
{
  // TODO:: We need to tell the Controller that it is already registered and there's no need to ask for registration.
  for (byte i = 0; i < 5; i++) {
    if(availableControllers[i] == nullptr)  {
      availableControllers[i] = controller;
      registeredControllersCount = registeredControllersCount + 1; // keep track of registered controllers.

      // TODO:: See if there's a better way to activate the first connected controller.
      if (registeredControllersCount == 1)
      {
        this->setActiveController(controller->controller.id);
      }

      return true; // found free slot. Returning true.
    }
  }
  // No free slots where found
  Serial.println("Maximum controllers have beed added already");
  return false;
}


// Debug

void ControllerManager::printActiveController()
{
  if (activeController != nullptr )
  {
    Serial.println("Active Controller ID :: ");
    Serial.print(activeController->controller.id[0]);
    Serial.print(" ");
    Serial.print(activeController->controller.id[1]);
    Serial.print(" ");
    Serial.print(activeController->controller.id[2]);
    Serial.print(" ");
    Serial.print(activeController->controller.id[3]);
    Serial.print(" ");
    Serial.println(activeController->controller.id[4]);
  }
}

void ControllerManager::printRegisteredControllers()
{
  if (registeredControllersCount > 0 && this->printInterval->check() == 1)
  {
    Serial.println();
    Serial.print("Registered controllers count:: ");
    Serial.print(registeredControllersCount);
    Serial.println();
    for (byte i = 0; i < registeredControllersCount; i++) {
      Serial.println("Controller ID :: ");
      Serial.print(availableControllers[i]->controller.id[0]);
      Serial.print(" ");
      Serial.print(availableControllers[i]->controller.id[1]);
      Serial.print(" ");
      Serial.print(availableControllers[i]->controller.id[2]);
      Serial.print(" ");
      Serial.print(availableControllers[i]->controller.id[3]);
      Serial.print(" ");
      Serial.println(availableControllers[i]->controller.id[4]);
    }
  }

}
