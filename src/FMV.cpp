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
  // Step 1. Check for a physical device that is trying to connect
//  if (stopScanning == false)
//  {
  this->connectionManager->handleClientConnections(); // detects new device and sets it as pending.
//  }

  // Step 2. Register physical devices as a controller
  this->handlePendingConnectionDevices(); // try to register the pending controllers if any are waiting.
  // // Step 3. Set active a controller.
  if(this->controllerManager->activeController != nullptr)
  {
    this->controllerManager->activeController->enable();
  }

  // // Step 4. Handle the Active Controller.
  this->controllerManager->handleController();
  //``
  // this->controllerManager->printRegisteredControllers();
  // this->controllerManager->printActiveController();

}


/**
  * Gets the pending registration devices from all connection interfaces and attempts to register them as controllers.
*/
void FMV::handlePendingConnectionDevices()
{

  if (this->connectionManager->pendingDevice.pending == true)
  {
   bool registered = this->controllerManager->registerController(this->connectionManager->pendingDevice);
   if (registered)
   {
     // We need an abstrac connection class here with virtual reset functions
     //this->connectionManager->radio->initPackets(); // nullify the packets.
     this->connectionManager->radio->clearPendingDevice(); // zerofiy the pendingDevice.
     stopScanning = true; // TEMP
   }
  }
  // TODO:: Make an array of pending Devices so that we can loop through all or at least a set of them.
  // if (this->connectionManager->pendingDevices[1].isWaiting == true)
  // {
  //   this->controllerManager->registerController(this->connectionManager->pendingDevices[1].type, this->connectionManager->pendingDevices[1].id);
  // }
}
