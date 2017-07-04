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
  delay(1000);
  //this->radio = new Radio();
  //this->radio->setup();
  delay(1000);
  handleClientInterval = new Metro(_HANDLE_CLIENT_INTERVAL);
  this->clearPendingDevice();
  Serial.println("Finished ConnectionManager Setup");
}


void ConnectionManager::handleClientConnections()
{
  //Serial.println("::::  connectionManager.handleConnections :: START");

  if (handleClientInterval->check() == 1)
  {
    if ( this->wifi->handleClientConnections() == true )
    {
      Serial.println("Radio Detected New Pending Device");
      // TODO:: We have an issue the pending devices are being overwritten here.
      this->pendingDevice = this->wifi->pendingDevice;
    }

    // if (this->radio->handleClientConnections() == true)
    // {
    //   Serial.println("Radio Detected New Pending Device");
    //   Serial.println(":::::::");
    //
    //   // Copy the pending Device so that we can work with it.
    //   pendingDevice = radio->pendingDevice;
    //
    //   Serial.print("ID: ");
    //   Serial.print(pendingDevice.id[0]);
    //   Serial.print(" ");
    //   Serial.print(pendingDevice.id[1]);
    //   Serial.print(" ");
    //   Serial.print(pendingDevice.id[2]);
    //   Serial.print(" ");
    //   Serial.print(pendingDevice.id[3]);
    //   Serial.print(" ");
    //   Serial.print(pendingDevice.id[4]);
    //   Serial.print(" ");
    //   Serial.println(pendingDevice.id[5]);
    //
    //   Serial.print("ADDRESS: ");
    //   Serial.print(pendingDevice.address[0]);
    //   Serial.print(" ");
    //   Serial.print(pendingDevice.address[1]);
    //   Serial.print(" ");
    //   Serial.print(pendingDevice.address[2]);
    //   Serial.print(" ");
    //   Serial.print(pendingDevice.address[3]);
    //   Serial.print(" ");
    //   Serial.print(pendingDevice.address[4]);
    //   Serial.print(" ");
    //   Serial.println(pendingDevice.address[5]);
    //
    //   Serial.print("CHANNEL: ");
    //   Serial.println(pendingDevice.channel);
    //
    //
    //   Serial.print("PENDING: ");
    //   Serial.println(pendingDevice.pending);
    //
    //
    //   // Clear the pending deivce on the radio object.
    //   radio->clearPendingDevice();
    // }
  }

  //Serial.println("::::  connectionManager.handleConnections :: FINISH");

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
  pendingDevice.address[0] = 0;
  pendingDevice.address[1] = 0;
  pendingDevice.address[2] = 0;
  pendingDevice.address[3] = 0;
  pendingDevice.address[4] = 0;
  pendingDevice.address[5] = 0;
  pendingDevice.channel = 0;
  pendingDevice.pending= false;

}

//TODO:: make the connection resources private. Write getters for them.
