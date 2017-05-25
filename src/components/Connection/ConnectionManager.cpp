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
  //TODO:: Maybe here we get a packats from the handle connection and pass it to the console.
  //this->wifi->handleClientConnections(); // TODO:: return Controller or No Controller
  this->radio->handleClientConnections(); // TODO: return COntroller or No Cotnroller
  //TODO:: reutrn array of Wifi or Radio Device.
  // pass response and source to Console.
  Serial.println("Finished connectionManager.handleConnections");

}


//TODO:: make the connection resources private. Write getters for them.
