#include <Arduino.h> // for the min
#include "components/Utility/Log.h"
#include "NunchuckController.h"
#include "../AbstractController.h"
#define min(a,b) ((a)<(b)?(a):(b)) // TODO:: Figure out why we don't get the min() function here.


// Construct the NunchuckController and the AbstractController
NunchuckController::NunchuckController(ConfigController* configController, Radio* radio, RadioDevice device)
 : AbstractController(configController, device)
{

  this->radio = radio;
  this->nunchuck = device;

  /**
   * Setiing the Metro Timers
   */
  this->connectionLostTimer = new Metro(_LOST_CONNECTION);         // read data interval


  // Init.
  responsePacket.Id      = 0;
  responsePacket.Command = 0;
  responsePacket.Value1  = 0;
  responsePacket.Value2  = 0;
  responsePacket.Value3  = 0;
  responsePacket.Value4  = 0;
  responsePacket.Value5  = 0;

  // Init.
  requestPacket.Id      = 0;
  requestPacket.Command = 0;
  requestPacket.Value1  = 0;
  requestPacket.Value2  = 0;
  requestPacket.Value3  = 0;
  requestPacket.Value4  = 0;
  requestPacket.Value5  = 0;

}


bool NunchuckController::handleController()
{

  if (this->radio->tryReadBytes(&responsePacket)) // populate the responsePacket
  {
    this->connectionLostTimer->reset();
    //this->printResponsePacket();
    this->processResponse(); // populate the requestPacket
  }
  //this->printRequestPacket();
  this->radio->tryWriteBytes(&requestPacket);
  // connectionLostTimer per each physical device
  if (this->connectionLostTimer->check() == 1)
  {
    Serial.println(":::::::::::::::::::::::::");
    Serial.println("CONNECTION LOST TO NunchuckController");
    Serial.println(":::::::::::::::::::::::::");
    return false;
  }

  return true;
}


void NunchuckController::processResponse()
{
  // This gets triggered only on active controller. So it is safe to take the physical controller out of idle mode;
  if(responsePacket.Command == 44)
  {
    Serial.println("Physical controller still in IDLE mode. Requesting Inputs");
    requestPacket.Command = 50; // request input values.
  }
  else if (responsePacket.Command == 55)
  {
    Serial.print(":::::::::::::: Controller inputs = ");
    Serial.print(responsePacket.Value2);
    Serial.println();
    processInput(responsePacket.Value2);
  }
}




bool NunchuckController::enable()
{
    this->radio->changeDevice(nunchuck);
    requestPacket.Command = 50;
}


//
// void NunchuckController::handleController()
// {
//   if (radio->_receiver->failureDetected)
//   {
//     Serial.println("RF24 Failure Detected. Re-running the setup.");
//     this->setup();
//   }
//   else
//   {
//     if (metroCommunication->check() == 1)
//     {
//       // Read data from transmitter
//       this->read();
//       this->printResponsePacket();
//
//       // Check performance of messages
//       if (metroController->check() == 1)
//       {
//         int rc = (_SIGNAL_CHECK_INTERVAL / _READ_INTERVAL) - 1;
//         radio->connectionStrength = min(float(receiveCounter) / float(rc) * 100, 100);
//         receiveCounter = 0;
//       }
//       //Serial.print("Connection Strength:: ");
//       //Serial.println(radio->connectionStrength);
//
//       // Write data to transmitter
//       //Serial.println("****************START WRITE************");
//       this->write();
//       this->printRequestPacket();
//       //Serial.println("****************END WRITE************");
//       if (metroHasController->check() == 1)
//       {
//         //Check if we had connection problems
//         radio->resetConnection();
//         // moved from resetConnection() to be able to abstract Radio.h
//         sendCommand = 0;
//       }
//     }
//   }
// }


// NunchuckController::~NunchuckController()
// {
//   Serial.println("NUNCHUCK DESTROYED");
// }


//TODO:: remove the interface requirements

void NunchuckController::write()
{

}

void NunchuckController::read()
{

}




// Debug

void NunchuckController::printRequestPacket()
{
  Serial.println("Nunchuck Packet sent:: ");
  Serial.print(requestPacket.Id);
  Serial.print(" ");
  Serial.print(requestPacket.Command);
  Serial.print(" ");
  Serial.print(requestPacket.Value1);
  Serial.print(" ");
  Serial.print(requestPacket.Value2);
  Serial.print(" ");
  Serial.print(requestPacket.Value3);
  Serial.print(" ");
  Serial.print(requestPacket.Value4);
  Serial.print(" ");
  Serial.print(requestPacket.Value5);
  Serial.print(" ");

  Serial.print("WriteBytes: ");
  Serial.println(packetSize);
  Serial.println();
}

void NunchuckController::printResponsePacket()
{
  Serial.println("Nunchuck Packet received:: ");
  Serial.print(responsePacket.Id);
  Serial.print(" ");
  Serial.print(responsePacket.Command);
  Serial.print(" ");
  Serial.print(responsePacket.Value1);
  Serial.print(" ");
  Serial.print(responsePacket.Value2);
  Serial.print(" ");
  Serial.print(responsePacket.Value3);
  Serial.print(" ");
  Serial.print(responsePacket.Value4);
  Serial.print(" ");
  Serial.print(responsePacket.Value5);
  Serial.print(" ");
  Serial.println();

  Serial.print("WriteBytes: ");
  Serial.println(packetSize);
}

void NunchuckController::printAddresses()
{
  Serial.println();
  Serial.print("READ ADDRESS :: ");
  Serial.print(radio->currentAddresses[0][0]);
  Serial.print(" | ");
  Serial.print(radio->currentAddresses[0][1]);
  Serial.print(" | ");
  Serial.print(radio->currentAddresses[0][2]);
  Serial.print(" | ");
  Serial.print(radio->currentAddresses[0][3]);
  Serial.print(" | ");
  Serial.print(radio->currentAddresses[0][4]);
  Serial.print(" | ");

  Serial.println();
  Serial.print("Write ADDRESS :: ");
  Serial.print(radio->currentAddresses[1][0]);
  Serial.print(" | ");
  Serial.print(radio->currentAddresses[1][1]);
  Serial.print(" | ");
  Serial.print(radio->currentAddresses[1][2]);
  Serial.print(" | ");
  Serial.print(radio->currentAddresses[1][3]);
  Serial.print(" | ");
  Serial.print(radio->currentAddresses[1][4]);
  Serial.print(" | ");
  Serial.println();

}
