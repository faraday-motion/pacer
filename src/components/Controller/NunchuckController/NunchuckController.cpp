#include <Arduino.h> // for the min
#include "components/Utility/Log.h"
#include "NunchuckController.h"
#include "../AbstractController.h"
#define min(a,b) ((a)<(b)?(a):(b)) // TODO:: Figure out why we don't get the min() function here.


// Construct the NunchuckController and the AbstractController
NunchuckController::NunchuckController(ConfigController* configController, byte controllerType, byte controllerId)
 : AbstractController(configController, controllerType, controllerId),
 transmitterId{0, 0, 0, 0, 0}         // initialize transmitterId
{

  radio = new Radio();
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

void NunchuckController::setup()
{
  // Serial.println("Setting Up Nunchuck Controller");
  // // TODO:: See what kind of setup we need, if any.
  // Serial.println("Finished Setting Up Nunchuck Controller");
}

void NunchuckController::read()
{
  if (tryReadBytes())
  {
    Serial.print("RECEIVED COMMAND:: ");
    Serial.println(responsePacket.Command);
    if (responsePacket.Command == 5)
    {
      //We have a name packet
      Serial.println("Name recived");
      //START TODO:: Here we need to call the controllerManager.
      //If the name is not yet set, or if its the same as previously connected
      if (isNewOrKnownController())
      {
        controllerVerified = true;
        //Send a request for address change
        sendCommand = 10;
      }
      else
      {
        controllerVerified = false;
        //Continue requesting controller name
        sendCommand = 1;
      }
      //END TODO
    }
    else if (responsePacket.Command == 15)
    {
      //We have an adress change confirmation
      Serial.println("Address change confirmed");
      radio->setAddress(radio->foundAddresses);
      radio->openPipes();
      //Send a request for the channel change
      sendCommand = 20;
    }
    else if (responsePacket.Command == 20)
    {
      //We have a channel change request
      Serial.println("Channel change ");
      radio->changeChannel();
      sendCommand = 20; // moved from changeChannel() for abstracting the Radio
    }
    else if (responsePacket.Command == 25)
    {
      //We have a channel change confirmation
      Serial.println("Channel change confirmed");
      radio->setChannel(radio->channelFound);
      sendCommand = 50;
    }
    else if (responsePacket.Command == 55)
    {
      //Controller inputs
      receiveCounter++;
      Serial.println(":::::::::::::: Controller inputs ::::::::::::::");
      Serial.println(responsePacket.Value2);
      Serial.println(":::::::::::::::::::::::::::::::::::::::::::::::");
      processInput(responsePacket.Value2);
    }
  }
} // end read();

void NunchuckController::write()
{
  // requestName
  if (sendCommand == 1)
  {
    requestPacket.Command = 1;
    tryWriteBytes();
  }
  // request Address Change
  else if (sendCommand == 10)
  {
    requestPacket.Command = 10;
    requestPacket.Value1  = radio->foundAddresses[0];
    requestPacket.Value2  = radio->foundAddresses[1];
    requestPacket.Value3  = radio->foundAddresses[2];
    requestPacket.Value4  = radio->foundAddresses[3];
    requestPacket.Value5  = radio->foundAddresses[4];
    tryWriteBytes();
  }
  // request Channel change
  else if (sendCommand == 20)
  {
    requestPacket.Command = 20;
    requestPacket.Value1  = radio->channelFound;
    tryWriteBytes();
  }
  // request Control packet
  else if (sendCommand == 50)
  {
    requestPacket.Command = 50;
    tryWriteBytes();
  }
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

bool NunchuckController::isNewOrKnownController()
{
  //TODO:: THIS MIGHT NEED TO BE EXPORTED TO THE ControllerManager.h
  //Was there no controller connecter before

  if (transmitterId[0] == 0 && transmitterId[1] == 0 && transmitterId[2] == 0 && transmitterId[3] == 0 && transmitterId[4] == 0)
  {
    //Set the controllerid
    transmitterId[0] = responsePacket.Value1 ;
    transmitterId[1] = responsePacket.Value2 ;
    transmitterId[2] = responsePacket.Value3 ;
    transmitterId[3] = responsePacket.Value4 ;
    transmitterId[4] = responsePacket.Value5 ;
    return true;
  }
  //TODO:: THIS MIGHT NEED TO BE EXPORTED TO THE ControllerManager.h
  //If a controller was connected, allow connecting if the same
  if (responsePacket.Value1 != transmitterId[0] || responsePacket.Value2 != transmitterId[1] || responsePacket.Value3 != transmitterId[2] || responsePacket.Value4 != transmitterId[3] || responsePacket.Value5 != transmitterId[4])
  {
    //We only allow a connection if the previous controller is the same as the one trying to connect
    return false;
  }
  else
  {
    return true;
  }
}


bool NunchuckController::tryReadBytes()
{
  bool timeout = false;
  bool success = false;
  long started_waiting_at = millis(); // timestamp started waiting.
  while (!radio->_receiver->available())
  {
    yield();
    if (millis() - started_waiting_at > _TIMEOUT_READ)
    {
      timeout = true;
      Serial.println("Readtimeout reached.");
      break;
    }
  }

  if(!timeout)
  {
    success = true;
    controllerConnected = true;
    metroHasController->reset();
    //Reading bytes from transmitter
    radio->_receiver->read(&responsePacket, packetSize);
    lastPacketId = responsePacket.Id;
    yield();
  }
  else
  {
    Serial.println("Reading from controller failed. Reason: timeout");
  }
  return success;
} // end tryReadBytes

bool NunchuckController::tryWriteBytes()
{
  bool success = false;
  requestPacket.Id = sendCount;
  requestPacket.Command = sendCommand;
  //TODO:: See if here we also need to set the values..
  // Don't listen while writing
  radio->_receiver->stopListening();

  if(radio->_receiver->write(&requestPacket, packetSize, 1))
  {
    sendCount++;
    success = true;
  } else
  {
    Serial.println("WRITING BYTES FAILED");
  }
  // This function should be called as soon as transmission is finished to drop the radio back to STANDBY-I mode
  radio->_receiver->txStandBy();
  // Enable listeing after writing
  radio->_receiver->startListening();
  yield();
  return success;
}

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
