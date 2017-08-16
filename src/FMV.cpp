#include "FMV.h"
#include "components/Device/AbstractDevice.h" // TODO: Make a class out of this.
#include "components/Utility/Log.h"

FMV::FMV()
{
}

void FMV::setup() {
  Serial.println("Setting up the Faraday Motion Vehicle...");
  this->configController = new ConfigController;
  if (!this->configController->hasLoadedConfig)
  {
    Serial.println("ERROR:: FMV could not configure itself.");
  }
  else
  {
    this->connectionManager = new ConnectionManager(configController);
    this->connectionManager->setup();
    this->controllerManager = new ControllerManager(configController, connectionManager);
    this->registerWiredDevices(); // We need to have a safety check.
    Serial.println("Finished setting up the Faraday Motion Vehicle");
  }
}

void FMV::loop()
{
  // Connection Manager Loop
  this->connectionManager->loop();
  // Handle Newly Connected Devices
  this->handlePendingConnectionDevices(); // try to register the pending controllers if any are waiting.

  // Step 3. Set active a controller. // NOTE:: now we automatically enable the active controller if there's no other active controller
  if(this->controllerManager->activeController != nullptr)
  {
    this->controllerManager->activeController->enable();
  }
  // Step 4. Get motorcontroller values that are used by controllers.
  while (Serial.available() > 0) this->controllerManager->motorController->processUartByte(Serial.read());
  // // Step 4. Handle the Active Controller.
  byte status = this->controllerManager->handleController();
  if (status == 2) // Status 2 stands for unsuccess.
  {
    Serial.println("FMV detected lost connection on the active controller.");
    // We know that the connection was lost. What now?
    this->controllerManager->unsetActiveController(); // This makes sure that we don't ask the radio to change to the active Device anymore.
    // TODO: Have a metro timer. We want to give the original active controller a chance to reconnect.
    //this->controllerManager->tryOtherControllers();
    //TODO: We should ping the idle controllers every now and then. Or should we only ping them when we actually want to activate them?
  }
}

void FMV::registerWiredDevices()
{
  Config* config = this->configController->config;
  if (config->wiredDevicesCount > 0) {
    for (byte i = 0; i < config->wiredDevicesCount; i++)
    {
      if(config->wiredDevices[i].enabled == 1) {
        AbstractDevice wiredDevice(config->wiredDevices[i]);
        this->controllerManager->registerController(wiredDevice);
      }
    }
  }
}
/**
  * Gets the pending registration devices from all connection interfaces and attempts to register them as controllers.
*/
void FMV::handlePendingConnectionDevices()
{
  if (this->connectionManager->pendingDevice.pending == true)
  {
   bool registered = this->controllerManager->registerController(this->connectionManager->pendingDevice);
   if (!registered)
   {
     // We need an abstract connection class here with virtual reset functions
     //this->connectionManager->radio->initPackets(); // nullify the packets.
     this->connectionManager->clearPendingDevice(); // zerofiy the pendingDevice.
     stopScanning = true; // flag for stoping the scanning of new devies.
   }
  }
}
