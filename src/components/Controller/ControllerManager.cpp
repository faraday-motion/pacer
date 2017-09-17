#include "ControllerManager.h"
#include "components/Utility/Log.h"

#include "AccelController/AccelController.h"
#include "WiredController/WiredController.h"
#include "PhoneController/PhoneController.h"
#include "NunchuckController/NunchuckController.h"
#include "BalanceController/BalanceController.h"


ControllerManager::ControllerManager(ConfigController* configController, ConnectionManager* connectionManager)
{
  Log::Logger()->write(Log::Level::INFO, "Started Setting up the ControllerManager...");
  // TODO:: It seems a bit redundant to pass all these pointers to the controllerManager and then to the AbstractController. Investigate on how to optimize this.
  this->configController  = configController;
  this->connectionManager = connectionManager;

  activeController = nullptr;
  controllerReadInterval = new Metro(10);
  controllerReadInterval->reset(); // Just to be safe.
  printInterval = new Metro(500);

  for (byte i = 0; i < 5; i++)
  {
    availableControllers[i] = nullptr;
  }
  Log::Logger()->write(Log::Level::INFO, "Finished Setting up the ControllerManager.");
}

// Main loop of this object. Handles the active controller.
void ControllerManager::loop()
{
  byte status = this->handleActiveController();
  if (status == 2)
  {
    Serial.println("Connection Lost to controller");
    Log::Logger()->write(Log::Level::WARNING, "Lost Connection to the Active Controller. Unsetting the active controller");
    this->unsetActiveController();
  }
}


byte ControllerManager::handleActiveController()
{
  byte status = 0; // 0 - No activeController ; 1 - all good ; 2 - connection lost on activeController
  if (activeController != nullptr && controllerReadInterval->check() == 1){
    if (activeController->handleController()) {
      status = 1;
    }
    else
    {
      status = 2;
    }
  }
  return status;
}


/**
 * Gets the current active device and returns it's obejct pointer.
 * returns nullptr if no active is set.
**/

AbstractController* ControllerManager::getActiveControllerId()
{
  return this->activeController;
}


/**
 * Takes a registered controller id and sets it as active.
 * Calls the AbstractCotnroller->enable() method to let the controller receiver know it is active.
**/

bool ControllerManager::setActiveController(byte id[])
{
  if (this->activeController == nullptr)
  {
    Log::Logger()->write(Log::Level::DEBUG, " No active controller. Ready to set new active controller");

    int index = getControllerIndexById(id);

    if (index == -1)
    {
      Log::Logger()->write(Log::Level::DEBUG, "Controller is NOT registered and NOT authorized. Cannot set as acitve.");
      return false;
    }

    Log::Logger()->write(Log::Level::DEBUG, "Controller is registered and authorized. Setting it as active.");
    activeController = availableControllers[index];
    activeController->enable(); // Enable the activeController;
    return true;
  }

  Log::Logger()->write(Log::Level::DEBUG, " A controller is already active");
  return false;
}


bool ControllerManager::unsetActiveController()
{
  Log::Logger()->write(Log::Level::DEBUG, "The active controller is being and unset.");
  activeController = nullptr;
  if (this->activeController == nullptr)
  {
    Log::Logger()->write(Log::Level::DEBUG, "Confimerd the removal of the active controller.");
    return true;
  }
  return false;
}

bool ControllerManager::tryOtherControllers()
{
  if (this->activeController == nullptr)
  {
    // Loop through the registered controllers // IDEA:: Give priorities to the controllers.
    for (size_t i = 0; i < 5; i++) {
      if (this->availableControllers[i] != nullptr)
      {
        this->setActiveController(this->availableControllers[i]->controller.id);
      }
    }
  }
  return false;
}


// TODO:: BUG:: After deleting the availableController Object we are still able to access it via the pointer.
void ControllerManager::removeRegisteredController(byte id[])
{
  Log::Logger()->write(Log::Level::DEBUG, "Uregistered a controller");
  byte index = this->getControllerIndexById(id);
  delete this->availableControllers[index];
  this->availableControllers[index] = nullptr;
}

bool ControllerManager::registerController(AbstractDevice device)
{
  // If the controller is not validated abort the registration and log event.
  Serial.println("Registering a controller");
  if (!this->validateController(device))
    return false;

  // // If it is registered and there's no active controller set as active.
  // if (this->activeController == nullptr)
  // {
  //   this->setActiveController(device.id);
  // }

  if (device.type == 1)
  {
    Log::Logger()->write(Log::Level::DEBUG, "Uregistered a controller");
    AbstractController * phoneController = new PhoneController(configController, connectionManager->wifi, device);
    allocateRegisteredController(phoneController);
    return true;
  }
  else if (device.type == 2)
  {
    Log::Logger()->write(Log::Level::DEBUG, "Registering a NunchuckControlle");
    AbstractController * nunchuckController = new NunchuckController(configController, connectionManager->radio, device);
    allocateRegisteredController(nunchuckController);
    return true;
  }
  else if (device.type == 3)
  {
    Log::Logger()->write(Log::Level::DEBUG, "Registering a AccelController");
    AbstractController * accelController = new AccelController(configController, device);
    allocateRegisteredController(accelController);
    return true;
  }
  else if (device.type == 4)
  {
    Log::Logger()->write(Log::Level::DEBUG, "Registering a WiredController");
    AbstractController * wiredController = new WiredController(configController, device);
    allocateRegisteredController(wiredController);
    return true;
  }
  else if (device.type == 5)
  {
    Log::Logger()->write(Log::Level::DEBUG, "Registering a BalanceController");
    AbstractController * balanceController = new BalanceController(configController, device);
    allocateRegisteredController(balanceController);
  }
  else
  {
    Log::Logger()->write(Log::Level::WARNING, "Can't register controller. Unknown type of controller.");
    return false;
  }
}

int ControllerManager::getControllerIndexById(byte id[])
{
  for (int i = 0; i < 5; i++)
  {
    if(availableControllers[i] != nullptr){  // not a null pointer
      if( availableControllers[i]->controller.id[0] == id[0] &&
          availableControllers[i]->controller.id[1] == id[1] &&
          availableControllers[i]->controller.id[2] == id[2] &&
          availableControllers[i]->controller.id[3] == id[3] &&
          availableControllers[i]->controller.id[4] == id[4]) {
        return i;
      }
    }
  }
  return -1;
}

// Stores the Abstract Cotnroller object in the array of registered controllers.
// TODO:: Implement priority of controllers here.
bool ControllerManager::allocateRegisteredController(AbstractController* controller)
{
  // TODO:: We need to tell the Controller that it is already registered and there's no need to ask for registration.
  for (byte i = 0; i < 5; i++) {
    if(availableControllers[i] == nullptr)  {
      availableControllers[i] = controller;
      registeredControllersCount = registeredControllersCount + 1; // keep track of registered controllers.
      this->printActiveController();
      // If there is no active controler we set this device as active.
      if (this->activeController == nullptr && controller->controller.enabled == true)
      {
        this->setActiveController(controller->controller.id);
        this->printActiveController();
      }

      return true; // found free slot. Returning true.
    }
  }
  // No free slots where found
  Log::Logger()->write(Log::Level::WARNING, "Maximum controllers have beed added already");
  return false;
}


bool ControllerManager::validateController(AbstractDevice device)
{
  // Stage One. Is this controller authorized?
  bool isAuthorized = false;
  Serial.println("Validating controller");
  for (byte i = 0; i < this->configController->config->authorizedControllersCount; i ++)
  {
    if (device.type == this->configController->config->authorizedControllers[i].type) {
      isAuthorized = true;
      break;
    }
  }

  if (!isAuthorized)
  {
    Log::Logger()->write(Log::Level::DEBUG, "This controller type is not authorized for this vehicle.");
    return false;
  }


  // Stage two. Is this controller already registered?
  if (this->getControllerIndexById(device.id) != -1 && isAuthorized)
  {
    Log::Logger()->write(Log::Level::DEBUG, "This controller was already registered");
    return false;
  }

  // All good. Proceed.
  return true;
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
