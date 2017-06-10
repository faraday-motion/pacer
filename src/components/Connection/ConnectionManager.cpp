#include "ConnectionManager.h"

ConnectionManager::ConnectionManager(ConfigController* configController)
{
  this->configController = configController;
  Serial.println("ConnectionManger Instantiated");
}

void ConnectionManager::setup()
{
  Serial.println("Started ConnectionManager Setup");

  this->wifi = new Wifi();
  this->wifi->setup(configController);

  this->radio = new Radio();
  this->radio->setup();

  handleClientInterval = new Metro(_HANDLE_CLIENT_INTERVAL);

  Serial.println("Finished ConnectionManager Setup");
}


// // This should be an abstract listener to connections to manage other connections then controllers.
// void ConnectionManager::handleClientConnections()
// {
//   //TODO:: Maybe here we get a packats from the handle connection and pass it to the console.
//   this->wifi->handleClientConnections(); // TODO:: return Controller or No Controller
//   this->radio->handleClientConnections(); // TODO: return COntroller or No Cotnroller
//
//   // pass response and source to Console.
//
// }



void ConnectionManager::handleClientConnections()
{
  Serial.println("Started connectionManager.handleConnections");

  if (handleClientInterval->check() == 1)
  {
    if ( this->wifi->handleClientConnections() == true ) // TODO:: return Controller or No Controller
    {
      // TODO:: The phone should be sending us this.
      pendingDevices[0].id[0] = 'W';
      pendingDevices[0].id[1] = 'I';
      pendingDevices[0].id[2] = 'F';
      pendingDevices[0].id[3] = 'I';
      pendingDevices[0].id[4] = '1';
      pendingDevices[0].type  =  1; // wifi
      pendingDevices[0].isWaiting = true;
    }

    if (this->radio->handleClientConnections() == true) // TODO: return Controller or No Cotnroller
    {
      Serial.println("Radio->handleClientConnections says there's a connection..");
      pendingDevices[1].id[0] = radio->pendingDeviceId[0];
      pendingDevices[1].id[1] = radio->pendingDeviceId[1];
      pendingDevices[1].id[2] = radio->pendingDeviceId[2];
      pendingDevices[1].id[3] = radio->pendingDeviceId[3];
      pendingDevices[1].id[4] = radio->pendingDeviceId[4];
      pendingDevices[1].type  = 2; // nunchuck
      pendingDevices[1].isWaiting = true;
    }
  }

  //TODO:: reutrn array of Wifi or Radio Device.
  // pass response and source to Console.
  Serial.println("Finished connectionManager.handleConnections");

}


void ConnectionManager::clearPendingDevices()
{
  pendingDevices[0].isWaiting = false;
  pendingDevices[1].isWaiting = false;
}

//TODO:: make the connection resources private. Write getters for them.
