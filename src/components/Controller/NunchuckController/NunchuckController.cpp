#include <Arduino.h> // for the min
#include "components/Utility/Log.h"
#include "NunchuckController.h"
#include "../AbstractController.h"
#define min(a,b) ((a)<(b)?(a):(b)) // TODO:: Figure out why we don't get the min() function here.


// Construct the NunchuckController and the AbstractController
NunchuckController::NunchuckController(ConfigController* configController, Radio* radio, byte controllerType, byte controllerId[])
 : AbstractController(configController, controllerType, controllerId),
 transmitterId{0, 0, 0, 0, 0}         // initialize transmitterId
{

  this->radio = radio;
  /**
    Setiing the Metro Timers
  */
  metroCommunication = new Metro(_READ_INTERVAL);         // read interval
  metroController    = new Metro(_SIGNAL_CHECK_INTERVAL); // signal check interval
  metroHasController = new Metro(500);
  metroChannelChange = new Metro(500);

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

void NunchuckController::read()
{
//
//     // Only this should be in the nunchuck
//     else if (responsePacket.Command == 55)
//     {
//       //Controller inputs
//       receiveCounter++;
//       Serial.println(":::::::::::::: Controller inputs ::::::::::::::");
//       Serial.println(responsePacket.Value2);
//       Serial.println(":::::::::::::::::::::::::::::::::::::::::::::::");
//       processInput(responsePacket.Value2);
//     }
//   }
} // end read();

void NunchuckController::write()
{
  // // requestName
  // if (sendCommand == 1)
  // {
  //   requestPacket.Command = 1;
  //   radio->tryWriteBytes(requestPacket);
  // }
  // // request Address Change
  // else if (sendCommand == 10)
  // {
  //   requestPacket.Command = 10;
  //   requestPacket.Value1  = radio->foundAddresses[0];
  //   requestPacket.Value2  = radio->foundAddresses[1];
  //   requestPacket.Value3  = radio->foundAddresses[2];
  //   requestPacket.Value4  = radio->foundAddresses[3];
  //   requestPacket.Value5  = radio->foundAddresses[4];
  //   radio->tryWriteBytes(requestPacket);
  // }
  // // request Channel change
  // else if (sendCommand == 20)
  // {
  //   requestPacket.Command = 20;
  //   requestPacket.Value1  = radio->channelFound;
  //   radio->tryWriteBytes(requestPacket);
  // }
  //
  // // TODO::
  // // request Control packet
  // else if (sendCommand == 50)
  // {
  //   requestPacket.Command = 50;
  //   radio->tryWriteBytes(requestPacket);
  // }
} // end write();

void NunchuckController::handleController()
{
  if (radio->_receiver->failureDetected)
  {
    Serial.println("RF24 Failure Detected. Re-running the setup.");
    this->setup();
  }
  else
  {
    if (metroCommunication->check() == 1)
    {
      // Read data from transmitter
      Serial.println("****************START READ************");
      this->read();
      this->printResponsePacket();
      Serial.println("****************END READ************");
      // Check performance of messages
      if (metroController->check() == 1)
      {
        int rc = (_SIGNAL_CHECK_INTERVAL / _READ_INTERVAL) - 1;
        radio->connectionStrength = min(float(receiveCounter) / float(rc) * 100, 100);
        receiveCounter = 0;
      }
      Serial.print("Connection Strength:: ");
      Serial.println(radio->connectionStrength);

      // Channel hop
      if (metroChannelChange->check() == 1)
      {
        // TODO:: Have a toggle switch fo this instead of commenting it out
        //if (controllerConnected && controllerVerified)
          //Serial.println("NOT SURE WHY BUT WE ARE CHANGING CHANNELS");
          //changeChannel();
      }

      // Write data to transmitter
      Serial.println("****************START WRITE************");
      this->write();
      this->printRequestPacket();
      Serial.println("****************END WRITE************");
      if (metroHasController->check() == 1)
      {
        //Check if we had connection problems
        radio->resetConnection();
        // moved from resetConnection() to be able to abstract Radio.h
        sendCommand = 0;
        // TODO:: this should be ported in the ControllerManager.h
        controllerConnected = false;
        controllerVerified  = false;
      }
    }
  }
}


/**
  Private Methods
*/
//
// bool NunchuckController::isNewOrKnownController()
// {
//   //TODO:: THIS MIGHT NEED TO BE EXPORTED TO THE ControllerManager.h
//   //Was there no controller connecter before
//
//   if (transmitterId[0] == 0 && transmitterId[1] == 0 && transmitterId[2] == 0 && transmitterId[3] == 0 && transmitterId[4] == 0)
//   {
//     //Set the controllerid
//     transmitterId[0] = responsePacket.Value1 ;
//     transmitterId[1] = responsePacket.Value2 ;
//     transmitterId[2] = responsePacket.Value3 ;
//     transmitterId[3] = responsePacket.Value4 ;
//     transmitterId[4] = responsePacket.Value5 ;
//     return true;
//   }
//   //TODO:: THIS MIGHT NEED TO BE EXPORTED TO THE ControllerManager.h
//   //If a controller was connected, allow connecting if the same
//   if (responsePacket.Value1 != transmitterId[0] || responsePacket.Value2 != transmitterId[1] || responsePacket.Value3 != transmitterId[2] || responsePacket.Value4 != transmitterId[3] || responsePacket.Value5 != transmitterId[4])
//   {
//     //We only allow a connection if the previous controller is the same as the one trying to connect
//     return false;
//   }
//   else
//   {
//     return true;
//   }
// }








void NunchuckController::printRequestPacket()
{
  Serial.println("Packet sent:: ");
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
  Serial.println("Packet received:: ");
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
