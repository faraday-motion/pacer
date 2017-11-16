#include "ControllerManager.h"
#include "components/Utility/Log.h"

#include "AccelController/AccelController.h"
#include "WiredController/WiredController.h"
#include "PhoneController/PhoneController.h"
#include "NunchuckController/NunchuckController.h"
#include "BalanceController/BalanceController.h"
#include "WsController/WsController.h"


ControllerManager::ControllerManager(ConnectionManager* connectionManager)
{
  Log::Logger()->write(Log::Level::INFO, "Started Setting up the ControllerManager...");
  this->connectionManager = connectionManager;

  activeController = nullptr;
  controllerReadInterval = new Metro(10);
  controllerReadInterval->reset(); // Just to be safe.
  printInterval = new Metro(500);

  for (byte i = 0; i < 5; i++)
  {
    registeredControllers[i] = nullptr;
  }
  Log::Logger()->write(Log::Level::INFO, "Finished Setting up the ControllerManager.");
}

// Main loop of this object. Handles the active controller.
void ControllerManager::loop()
{
  byte status = this->handleActiveController();
  if (status == 2)
  {
    Log::Logger()->write(Log::Level::WARNING, "Lost Connection to the Active Controller. Unsetting the active controller");
    this->unsetActiveController();
  }
}

byte ControllerManager::handleActiveController()
{
  byte status = 0; // 0 - No activeController ; 1 - all good ; 2 - connection lost on activeController
  if (activeController != nullptr && controllerReadInterval->check() == 1){
    if (activeController->handleController())
      status = 1;
    else
      status = 2;
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
    Log::Logger()->write(Log::Level::DEBUG, "No active controller. Ready to set new active controller");

    int index = getControllerIndexById(id);

    if (index == -1)
    {
      Log::Logger()->write(Log::Level::DEBUG, "Controller is NOT registered and NOT authorized. Cannot set as acitve.");
      return false;
    }

    Log::Logger()->write(Log::Level::DEBUG, "Controller is registered and authorized. Setting it as active.");
    activeController = registeredControllers[index];
    activeController->enable(); // Enable the activeController;
    return true;
  }

  Log::Logger()->write(Log::Level::DEBUG, " A controller is already active");
  return false;
}

// TODO:: Please make me look more beautiful and readable.
bool ControllerManager::unsetActiveController()
{
  Log::Logger()->write(Log::Level::DEBUG, "The active controller is being and unset.");
  // Check if there's an active controller at all.
  if (this->activeController != nullptr) {
    // Check if we are in motion and it is save to unset the controller.
    if(this->activeController->isInMotion){
      Log::Logger()->write(Log::Level::DEBUG, "Cannot unset the active controller. FMV is in motion");
      return false;
    }
  }
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
      if (this->registeredControllers[i] != nullptr)
      {
        this->setActiveController(this->registeredControllers[i]->controller.id);
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
  delete this->registeredControllers[index];
  this->registeredControllers[index] = nullptr;
}

bool ControllerManager::registerController(AbstractDevice device)
{
  // If the controller is not validated abort the registration and log event.
  if (!this->validateController(device))
    return false;

  // Stage two. Is this controller already registered?
  if (this->getControllerIndexById(device.id) != -1)
  {
    Log::Logger()->write(Log::Level::DEBUG, "This controller was already registered");

    Log::Logger()->write(Log::Level::DEBUG, "Checking if it could be activated");
    if(this->activeController == nullptr) {
      this->setActiveController(device.id);
      this->activeController->enable();
    }

    return true;
  }
  // TODO:: Make a freaking enum out of this.
  if (device.type == 1)
  {
    Log::Logger()->write(Log::Level::DEBUG, "Registering a PhoneController");
    AbstractController * phoneController = new PhoneController(connectionManager->wifi, device);
    allocateRegisteredController(phoneController);
    return true;
  }
  else if (device.type == 2)
  {
    Log::Logger()->write(Log::Level::DEBUG, "Registering a NunchuckControlle");
    AbstractController * nunchuckController = new NunchuckController(connectionManager->radio, device);
    allocateRegisteredController(nunchuckController);
    return true;
  }
  else if (device.type == 3)
  {
    Log::Logger()->write(Log::Level::DEBUG, "Registering a AccelController");
    AbstractController * accelController = new AccelController(device);
    allocateRegisteredController(accelController);
    return true;
  }
  else if (device.type == 4)
  {
    Log::Logger()->write(Log::Level::DEBUG, "Registering a WiredController");
    AbstractController * wiredController = new WiredController(device);
    allocateRegisteredController(wiredController);
    return true;
  }
  else if (device.type == 5)
  {
    Log::Logger()->write(Log::Level::DEBUG, "Registering a WsController");
    AbstractController * wsController = new WsController(connectionManager->ws, device);
    allocateRegisteredController(wsController);
    return true;
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
    if(registeredControllers[i] != nullptr){  // not a null pointer
      if( registeredControllers[i]->controller.id[0] == id[0] &&
          registeredControllers[i]->controller.id[1] == id[1] &&
          registeredControllers[i]->controller.id[2] == id[2] &&
          registeredControllers[i]->controller.id[3] == id[3] &&
          registeredControllers[i]->controller.id[4] == id[4]) {
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
    if(registeredControllers[i] == nullptr)  {
      registeredControllers[i] = controller;
      registeredControllersCount = registeredControllersCount + 1; // keep track of registered controllers.
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

  Config* config = Config::get();

  for (byte i = 0; i < config->authorizedControllersCount; i ++)
  {
    if (device.type == config->authorizedControllers[i].type) {
      isAuthorized = true;
      break;
    }
  }

  if (!isAuthorized)
  {
    Log::Logger()->write(Log::Level::DEBUG, "This controller type is not authorized for this vehicle.");
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
      Serial.print(registeredControllers[i]->controller.id[0]);
      Serial.print(" ");
      Serial.print(registeredControllers[i]->controller.id[1]);
      Serial.print(" ");
      Serial.print(registeredControllers[i]->controller.id[2]);
      Serial.print(" ");
      Serial.print(registeredControllers[i]->controller.id[3]);
      Serial.print(" ");
      Serial.println(registeredControllers[i]->controller.id[4]);
    }
  }

}
