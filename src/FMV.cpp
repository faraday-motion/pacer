#include "FMV.h"
#include "components/Device/AbstractDevice.h" // TODO: Make a class out of this.


FMV::FMV()
{
}

void FMV::setup() {
  Serial.println("Setting up the Faraday Motion Vehicle...");
  this->configController = new ConfigController;
  this->configController->loadConfig();
  this->connectionManager = new ConnectionManager(configController);
  this->connectionManager->setup();
  this->controllerManager = new ControllerManager(configController, connectionManager);
  //this->registerWiredDevices(); // We need to have a safety check.
  Serial.println("Finished setting up the Faraday Motion Vehicle");
}

void FMV::loop()
{
  // WebSocketLoop(); NOTE:: Maybe we can have it in another loop somewhere.
  this->connectionManager->ws->wss->loop();
  // HTTP Server Loop; NOTE:: We should group communication handling in a different loop
  this->connectionManager->webServer->handleClient();

  // Step 1. Check for a physical device that is trying to connect
  //this->connectionManager->handleClientConnections(); // detects new device and sets it as pending.
  // Step 2. Register physical devices as a controller
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
  if (status == 2) // Status 2 stands for success.
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
  byte count = 1;
  if (count > 0) {
    for (byte i = 0; i < count; i++)
    {
      AbstractDevice wiredDevice(config->wiredDevices[i]);
      this->controllerManager->registerController(wiredDevice);
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
