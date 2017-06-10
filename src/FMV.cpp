#include "FMV.h"

FMV::FMV()
{

}

void FMV::setup() {
  this->configController = new ConfigController;
  this->configController->loadConfig();
  this->connectionManager = new ConnectionManager(configController);
  this->connectionManager->setup();
  this->controllerManager = new ControllerManager(configController, connectionManager);
  printRegisteredControllers = new Metro(500);
}

void FMV::loop()
{

  // Step 1. Check for physical device that is trying to connect
  this->connectionManager->handleClientConnections(); // queue pending devices.
  // Step 2. Register physical devices as a controller
  this->registerPendingConnectionDevices(); // register the pending controllers is any are waiting.
  // Step 3. Set active a controller.

  // Step 4. Handle the Active Controller.
  this->controllerManager->handleController();
  if (this->controllerManager->registeredControllersCount > 0 && printRegisteredControllers->check() == 1)
  {
    Serial.println();
    Serial.print("Registered controllers count:: ");
    Serial.print(this->controllerManager->registeredControllersCount);
    for (byte i = 0; i < this->controllerManager->registeredControllersCount; i++) {
      Serial.println("Controller ID");
      Serial.print(this->controllerManager->availableControllers[i]->controllerId[0]);
      Serial.print(" ");
      Serial.print(this->controllerManager->availableControllers[i]->controllerId[1]);
      Serial.print(" ");
      Serial.print(this->controllerManager->availableControllers[i]->controllerId[2]);
      Serial.print(" ");
      Serial.print(this->controllerManager->availableControllers[i]->controllerId[3]);
      Serial.print(" ");
      Serial.println(this->controllerManager->availableControllers[i]->controllerId[4]);
    }
  }

}

void FMV::registerPendingConnectionDevices()
{

  if (this->connectionManager->pendingDevices[0].isWaiting == true)
  {
    Serial.println("R1");
   this->controllerManager->registerController(this->connectionManager->pendingDevices[0].type, this->connectionManager->pendingDevices[0].id);
  }

  if (this->connectionManager->pendingDevices[1].isWaiting == true)
  {
    Serial.println("R2");
    this->controllerManager->registerController(this->connectionManager->pendingDevices[1].type, this->connectionManager->pendingDevices[1].id);
  }
}
