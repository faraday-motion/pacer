#include "ConnectionManager.h"
#include "components/Utility/Log.h"

ConnectionManager::ConnectionManager(ConfigController* configController)
{
  this->configController = configController;
}

void ConnectionManager::setup()
{
  Log::Logger()->write(Log::Level::INFO, "Started ConnectionManager Setup");
  Config* config = this->configController->config;
  handleClientInterval = new Metro(_HANDLE_CLIENT_INTERVAL);

  this->clearPendingDevice();

  // Nullify optional modules pointers.
  this->ws        = nullptr;
  this->radio     = nullptr;
  this->webServer = nullptr;

  // Setup Wifi
  this->wifi = new Wifi();
  this->wifi->setup(configController);
  delay(100);

  // Setup WebServer
  if (config->modules.webServer)
  {
    this->webServer = new WebServer();
    this->webServer->setup();
  }

  // Setup NRF24
  if(config->modules.radio)
  {
    this->radio = new Radio();
    this->radio->setup();
    delay(100);
  }

  //Setup WebSocketCommunicator
  if(config->modules.webSocketServer)
  {
    this->ws = new WebSocketCommunicator(configController); // TODO:: The port should be configurable
    this->ws->wss->begin();
    delay(100);
  }
  //
  // // Bind logger to websockets
  // Log::log->enableWebsocket(this->ws);

  Log::Logger()->write(Log::Level::INFO, "Finished ConnectionManager Setup");
}

// A loop that groups all the looped methods of different connection interfaces.
void ConnectionManager::loop()
{
  //Websocket loop
  if (this->ws != nullptr)
  {
    this->ws->wss->loop();
  }
  //HTTP Server Loop
  if (this->webServer != nullptr)
  {
    this->webServer->handleClient();
  }
  // Handle Client Connections
  this->handleClientConnections();
}


void ConnectionManager::handleClientConnections()
{
  if (handleClientInterval->check() == 1)
  {
    this->handleWifiConnections();
    this->handleRadioConnections();
  }
}

/** Checks if wifi is instantiated and checks for new clinets wanting to connect */
void ConnectionManager::handleWifiConnections()
{
  if (this->wifi != nullptr)
  {
    if (this->wifi->handleClientConnections() == true) {
      Log::Logger()->write(Log::Level::DEBUG, "Wifi Detected New Pending Device");
      // TODO:: We have an issue the pending devices are being overwritten here by handleRadioConnections()
      this->pendingDevice = this->wifi->pendingDevice;
    }
  }
}

/** Checks if radio is instantiated and checks for new clinets wanting to connect */
void ConnectionManager::handleRadioConnections()
{
  if (this->radio != nullptr)
  {
    if (this->radio->handleClientConnections() == true)
    {
      Log::Logger()->write(Log::Level::DEBUG, "Radio Detected New Pending Device");
      // Copy the pending Device so that FMV can work with it.
      this->pendingDevice = this->radio->pendingDevice;
      // Clear the pending deivce on the radio object.
      this->radio->clearPendingDevice();
    }
  }

}

/**
 * Clear the 2 element array that is storing the pending IDs and types of devices requesting to be registered.
 */
void ConnectionManager::clearPendingDevice()
{
  pendingDevice.id[0] = 0;
  pendingDevice.id[1] = 0;
  pendingDevice.id[2] = 0;
  pendingDevice.id[3] = 0;
  pendingDevice.id[4] = 0;
  pendingDevice.id[5] = 0;

  pendingDevice.pending = false;
  pendingDevice.priority = 0;

  pendingDevice.address[0] = 0;
  pendingDevice.address[1] = 0;
  pendingDevice.address[2] = 0;
  pendingDevice.address[3] = 0;
  pendingDevice.address[4] = 0;
  pendingDevice.address[5] = 0;
  pendingDevice.channel = 0;

}
