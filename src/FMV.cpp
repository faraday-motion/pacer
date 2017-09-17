#include "FMV.h"
#include "components/Device/AbstractDevice.h" // TODO: Make a class out of this.
#include "components/Utility/Log.h"

FMV::FMV()
{
}

void FMV::setup() {

  Log::Logger()->enable();
  Log::Logger()->setLevel(Log::Level::DEBUG);

  Log::Logger()->write(Log::Level::INFO, "Setting up the Faraday Motion Vehicle...");
  Log::Logger()->write(Log::Level::DEBUG, "Setting the console");
  Console::Cmd()->setFMV(this);

  this->configController = new ConfigController;
  this->wasConfigured = this->configController->hasLoadedConfig;

  if (this->wasConfigured) {
    this->connectionManager = new ConnectionManager(configController);
    this->connectionManager->setup();
    this->controllerManager = new ControllerManager(configController, connectionManager);
    this->registerControllers(); // We need to have a safety check.
    Log::Logger()->write(Log::Level::INFO, "Finished setting up the Faraday Motion Vehicle");
  }
}

void FMV::loop()
{
  if (this->wasConfigured)
  {
    // Connection Manager Loop
    this->connectionManager->loop();

    // Handle Newly Connected Devices
    this->handlePendingConnectionDevices(); // try to register the pending controllers if any are waiting.

    // Conntroller Manager Loop
    this->controllerManager->loop();
    // Read Data from VESC  // TODO:: This should be moved somewhere else.
    while (Serial.available() > 0) this->controllerManager->motorController->processUartByte(Serial.read());
  }
  else
  {
    Log::Logger()->write(Log::Level::ERR, "FATAL: Faraday Motion Pacer Vehicle was not correctly configured.");
    delay(2000);
  }
}

/***
   * Register all enabled devices from the configuration.
***/
void FMV::registerControllers()
{
  Config* config = this->configController->config;
  //config->printRegisteredControllers();
  if (config->registeredControllersCount > 0) {
    for (byte i = 0; i < config->registeredControllersCount; i++)
    {
      AbstractDevice registeredController(config->registeredControllers[i]);
      this->controllerManager->registerController(registeredController);
    }
  }
}

/**
  * Gets the devices that are pending registration from all connection interfaces and attempts to register them as controllers.
*/
void FMV::handlePendingConnectionDevices()
{
  if (this->connectionManager->pendingDevice.pending == true)
  {
   bool registered = this->controllerManager->registerController(this->connectionManager->pendingDevice);
   if (!registered)
   {
     this->connectionManager->clearPendingDevice(); // zerofy the pendingDevice.
   }
  }
}
