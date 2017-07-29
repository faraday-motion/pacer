#include "ConnectionManager.h"
#include "components/Utility/Log.h"

ConnectionManager::ConnectionManager(ConfigController* configController)
{
  this->configController = configController;
  Serial.println("ConnectionManger Instantiated");
}

void ConnectionManager::setup()
{
  Serial.println("Started ConnectionManager Setup");

  this->clearPendingDevice();
  handleClientInterval = new Metro(_HANDLE_CLIENT_INTERVAL);

  // Setup Wifi
  this->wifi = new Wifi();
  this->wifi->setup(configController);
  delay(500);

  // Setup WebServer
  this->webServer = new WebServer();
  this->webServer->setup();

  // Setup NRF24
  this->radio = new Radio();
  this->radio->setup();
  delay(500);

  //Setup WebSocketCommunicator
  this->ws = new WebSocketCommunicator(81); // TODO:: The port should be configurable
  this->ws->wss->begin();
  delay(100);

  // Bind logger to websockets
  Log::Instance()->enableWebsocket(this->ws);


  Serial.println("Finished ConnectionManager Setup");
}


void ConnectionManager::handleClientConnections()
{

  if (handleClientInterval->check() == 1)
  {
    if ( this->wifi->handleClientConnections() == true )
    {
      Serial.println("Wifi Detected New Pending Device");
      // TODO:: We have an issue the pending devices are being overwritten here.
      this->pendingDevice = this->wifi->pendingDevice;
    }

    if (this->radio->handleClientConnections() == true)
    {
      Serial.println("Radio Detected New Pending Device");

      // Copy the pending Device so that FMV can work with it.
      pendingDevice = radio->pendingDevice;

      // Clear the pending deivce on the radio object.
      radio->clearPendingDevice();
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
