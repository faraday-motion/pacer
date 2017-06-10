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

}

void FMV::loop()
{

  // Step 1. Check for physical device that is trying to connect
  this->connectionManager->handleClientConnections(); // POPULATE NEW DEVICES
  // Step 2. Register physical devices as a controller

  // CASE:: WIFI
  // if (newDevices[0][0] != 0){
  //   wasRegistered = this->controllerManager->registerController(newDevices[0]);
  //   if (wasRegistered == true)
  //   {
  //     this->connectionManager->wifi->tryWrite(); // Write Confirmation.
  //   }
  // }

  // CASE:: RADIO
  bool registered = false;
  if (this->connectionManager->radio->transmitterId[0] != 0) {
    registered = this->controllerManager->registerController(2, this->connectionManager->radio->transmitterId);
  }

  if (registered == true)
  {
    Serial.println(":::::::::Controller Registered:::::::");
    Serial.println("Controller ID");
    Serial.print(this->controllerManager->availableControllers[0]->controllerId[0]);
    Serial.print(" ");
    Serial.print(this->controllerManager->availableControllers[0]->controllerId[1]);
    Serial.print(" ");
    Serial.print(this->controllerManager->availableControllers[0]->controllerId[2]);
    Serial.print(" ");
    Serial.print(this->controllerManager->availableControllers[0]->controllerId[3]);
    Serial.print(" ");
    Serial.println(this->controllerManager->availableControllers[0]->controllerId[4]);
  }

  this->controllerManager->handleController();
}
