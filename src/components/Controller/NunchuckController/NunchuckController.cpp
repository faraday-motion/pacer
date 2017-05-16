#include <Arduino.h> // for the min
#include "components/Utility/Log.h"
#include "NunchuckController.h"
#include "../AbstractController.h"
#define min(a,b) ((a)<(b)?(a):(b)) // TODO:: Figure out why we don't get the min() function here.


// Construct the NunchuckController and the AbstractController
NunchuckController::NunchuckController(ConfigController* configController, MotorController* motorController, byte controllerType, byte controllerId)
 : AbstractController(configController, motorController, controllerType, controllerId),
 defaultAddresses{"FM01R", "FM01W"},  // initialize defaultAddresses
 currentAddresses{"FM01R", "FM01W"},  // initialize currentAddresses
 transmitterId{0, 0, 0, 0, 0}         // initialize transmitterId
{
  /**
    Setiing the Metro Timers
  */
  metroCommunication = new Metro(_READ_INTERVAL);         // read interval
  metroController    = new Metro(_SIGNAL_CHECK_INTERVAL); // signal check interval
  metroHasController = new Metro(500);
  metroChannelChange = new Metro(500);

  byte foundAddresses[6] = {"FM000"}; // TODO:: this is a bad way for initializing the array.


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

  this->_receiver = new RF24(4,15); // TODO:: These are environment pin configurations. Should get it out of the .ini file.
  this->setup();
}

void NunchuckController::setup()
{
  Serial.println("Setting Up Nunchuck Controller");
  _receiver->begin();
  _receiver->setAutoAck(false);
  _receiver->setRetries(0, 0);
  //_receiver->setDataRate(RF24_250KBPS);
  //_receiver->setPALevel(RF24_1MBPS);
  _receiver->setPALevel(RF24_1MBPS); // Uncomment for more power
  _receiver->setPayloadSize(packetSize);
   this->setChannel(channelDefault); // we also set the currentChannel member here.
  _receiver->failureDetected = 0;
  _receiver->printDetails();

  delay(50);
  findChannel();
  delay(50);
  setAddress(defaultAddresses[0]);
  delay(50);
  //generateRandomAddress();
  openPipes();
  Serial.println("Finished Setting Up Nunchuck Controller");

}

// handleRead() in the old code //TODO:: remove this comment.
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
    }
    else if (responsePacket.Command == 15)
    {
      //We have an adress change confirmation
      Serial.println("Address change confirmed");
      setAddress(foundAddresses);
      openPipes();
      //Send a request for the channel change
      sendCommand = 20;
    }
    else if (responsePacket.Command == 20)
    {
      //We have a channel change request
      Serial.println("Channel change ");
      changeChannel();
    }
    else if (responsePacket.Command == 25)
    {
      //We have a channel change confirmation
      Serial.println("Channel change confirmed");
      setChannel(channelFound);
      sendCommand = 50;
    }
    else if (responsePacket.Command == 55)
    {
      //Controller inputs
      receiveCounter++;
      Serial.print(":::::::::::::: Controller inputs ::::::::::::::");
      Serial.println(responsePacket.Value2);
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
    requestPacket.Value1  = foundAddresses[0];
    requestPacket.Value2  = foundAddresses[1];
    requestPacket.Value3  = foundAddresses[2];
    requestPacket.Value4  = foundAddresses[3];
    requestPacket.Value5  = foundAddresses[4];
    tryWriteBytes();
  }
  // request Channel change
  else if (sendCommand == 20)
  {
    requestPacket.Command = 20;
    requestPacket.Value1  = channelFound;
    tryWriteBytes();
  }
  // request Control packet
  else if (sendCommand == 50)
  {
    Serial.println("Sending Command :: 50");
    requestPacket.Command = 50;
    tryWriteBytes();
  }
} // end write();

void NunchuckController::listenToController()
{
  if (_receiver->failureDetected)
  {
    Serial.println("RF24 Failure Detected. Re-running the setup.");
    this->setup();
  }
  else
  {
    if (metroCommunication->check() == 1)
    {
      // Read data from transmitter
      Serial.println();
      Serial.println();
      Serial.println("****************START READ************");
      this->read();
      this->printResponsePacket();
      Serial.println("****************END READ************");
      // Check performance of messages
      if (metroController->check() == 1)
      {
        int rc = (_SIGNAL_CHECK_INTERVAL / _READ_INTERVAL) - 1;
        connectionStrength = min(float(receiveCounter) / float(rc) * 100, 100);
        receiveCounter = 0;
      }
      Serial.print("Connection Strength:: ");
      Serial.println(connectionStrength);

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
        resetConnection();
      }
    }
  }
}

/**
  Private Methods
*/

void NunchuckController::findChannel()
{
  byte currentChannel = channelSelected;

  for (byte i = channelMax; i >= channelMax; i--)
  {
    setChannel(i);
    _receiver->startListening();

    //We need to wait to get proper results from the tests
    delayMicroseconds(750);

    if(!_receiver->testCarrier())
    {
      //Let's make sure that the next channel is a different one than teh last one and that there is some badnwith between
      if (currentChannel != i && abs(currentChannel - 1) > 6) // The value 6 is a offset for searching channels that are 6 channels away from the previous channelSelected.
      {
        channelFound = i;
        break;
      }
    }
    yield();
  } // end for loop.

  //If a good channel is not found, continue with the default one
  if (channelFound == 255)
    channelFound = channelDefault;

  _receiver->stopListening();
  setChannel(currentChannel);

}

void NunchuckController::setChannel(byte channel)
{

  _receiver->setChannel(channel);
  channelSelected = channel;
}

void NunchuckController::generateRandomAddress()
{
  randomSeed(analogRead(A0));
  foundAddresses[0] = random(0, 255);
  foundAddresses[1] = random(0, 255);
  foundAddresses[2] = random(0, 255);
  foundAddresses[3] = random(0, 255);
  foundAddresses[4] = random(0, 255);
}

//Note the address is mirrored with the other transiever
void NunchuckController::setAddress(byte address[])
{
  // Here we assemble address;
  for (byte i = 0; i < 5; i++)
  {
    currentAddresses[0][i] = address[i];
    currentAddresses[1][i] = address[i];
  }

  currentAddresses[0][4] = 'R';
  currentAddresses[1][4] = 'W';
}

void NunchuckController::openPipes()
{
  _receiver->openReadingPipe(1, currentAddresses[0]);
  _receiver->openWritingPipe(currentAddresses[1]);
  _receiver->startListening();
}

bool NunchuckController::isNewOrKnownController()
{
  //TODO:: THIS MIGHT NEED TO BE EXPORTED TO THE ControllerManager.h
  //Was there no controller connecter before
  Serial.println("::::: isNewOrKnownController ::::");
  Serial.println("Current controller :: ");
  Serial.print(transmitterId[0]);
  Serial.print("|");
  Serial.print(transmitterId[1]);
  Serial.print("|");
  Serial.print(transmitterId[2]);
  Serial.print("|");
  Serial.print(transmitterId[3]);
  Serial.print("|");
  Serial.print(transmitterId[4]);
  Serial.print("|");
  Serial.println();

  if (transmitterId[0] == 0 && transmitterId[1] == 0 && transmitterId[2] == 0 && transmitterId[3] == 0 && transmitterId[4] == 0)
  {
    //Set the controllerid
    transmitterId[0] = responsePacket.Value1 ;
    transmitterId[1] = responsePacket.Value2 ;
    transmitterId[2] = responsePacket.Value3 ;
    transmitterId[3] = responsePacket.Value4 ;
    transmitterId[4] = responsePacket.Value5 ;
    Serial.println("Controller is was set to :: ");
    Serial.print(responsePacket.Value1);
    Serial.print(" ");
    Serial.print(responsePacket.Value2);
    Serial.print(" ");
    Serial.print(responsePacket.Value3);
    Serial.print(" ");
    Serial.print(responsePacket.Value4);
    Serial.print(" ");
    Serial.print(responsePacket.Value5);
    Serial.println(" ");
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

void NunchuckController::changeChannel()
{
  Serial.println("changeChannel");
  //Bug nee to call findChannel() 2 times for it to work. investigate.
  findChannel();
  sendCommand = 20;

}

bool NunchuckController::tryReadBytes()
{
  bool timeout = false;
  bool success = false;
  long started_waiting_at = millis(); // timestamp started waiting.
  while (!_receiver->available())
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
    Serial.println("*** Reading packet ***");
    success = true;
    controllerConnected = true;
    metroHasController->reset();
    //TODO:: Implement metroHasController.reset();
    metroHasController->reset();
    //Reading bytes from transmitter
    _receiver->read(&responsePacket, packetSize); //TODO:: Are we sure this will

    lastPacketId = responsePacket.Id;
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
  _receiver->stopListening();

  if(_receiver->write(&requestPacket, packetSize, 1))
  {
    sendCount++;
    success = true;
  } else
  {
    Serial.println("WRITING BYTES FAILED");
  }
  // This function should be called as soon as transmission is finished to drop the radio back to STANDBY-I mode
  _receiver->txStandBy();
  // Enable listeing after writing
  _receiver->startListening();
  return success;
}

void NunchuckController::resetConnection()
{
  Serial.println("START RESET CONN");
  controllerConnected = false;
  controllerVerified  = false;
  receiveCounter = 0;
  setChannel(channelDefault);
  setAddress(defaultAddresses[0]);
  generateRandomAddress();
  openPipes();
  sendCommand = 0;
  Serial.println("END RESET CONN");
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
