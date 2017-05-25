#include <Arduino.h>
#include "Radio.h"
#include "components/Utility/Log.h"

Radio::Radio() :
defaultAddresses{"FM01R", "FM01W"},  // initialize defaultAddresses
currentAddresses{"FM01R", "FM01W"},  // initialize currentAddresses
foundAddresses{"FM000"},             // initialize foundAddresses
transmitterId{0, 0, 0, 0, 0}
{
  this->_receiver = new RF24(4,15); // TODO:: These are environment pin configurations. Should get it out of the .ini file.
}

void Radio::setup()
{
  Serial.println("Setting Up Radio Connection");
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
  generateRandomAddress();
  openPipes();
  Serial.println("Finished Setting Up Radio Connection");
  yield();
}


void Radio::handleClientConnections()
{
  Serial.println(":::::::::Started radio.handleConnections");
  if (_receiver->failureDetected)
  {
    Serial.println("RF24 Failure Detected. Re-running the setup.");
    this->setup();
    this->handleClientConnections(); // Retry
  }

  if (tryReadBytes())
  {
    handShaking = true;
    if (responsePacket.Command == 5)
    {
      //We have a name packet
      Serial.println("Name recived");

      transmitterId[0] = responsePacket.Value1 ;
      transmitterId[1] = responsePacket.Value2 ;
      transmitterId[2] = responsePacket.Value3 ;
      transmitterId[3] = responsePacket.Value4 ;
      transmitterId[4] = responsePacket.Value5 ;


      //Send a request for address change
      requestPacket.Command = 10;
      requestPacket.Value1  = this->foundAddresses[0];
      requestPacket.Value2  = this->foundAddresses[1];
      requestPacket.Value3  = this->foundAddresses[2];
      requestPacket.Value4  = this->foundAddresses[3];
      requestPacket.Value5  = this->foundAddresses[4];

    }
    else if (responsePacket.Command == 15)
    {
      //We have an adress change confirmation
      Serial.println("Address change confirmed");
      this->setAddress(this->foundAddresses);
      this->openPipes();
      //Send a request for the channel change
      requestPacket.Command = 20;
      requestPacket.Value1  = this->channelFound;
    }
    else if (responsePacket.Command == 20)
    {
      //We have a channel change request
      Serial.println("Channel change ");
      this->changeChannel();
      requestPacket.Command = 20; // moved from changeChannel() for abstracting the Radio
    }
    else if (responsePacket.Command == 25)
    {
      //We have a channel change confirmation
      Serial.println("Channel change confirmed");
      this->setChannel(this->channelFound);
      handShaking = false;
      //sendCommand = 50; //TODO:: This command should only be sent after we have registered and Activated the controller.
    }
  }

  Serial.println();
  Serial.print("handShaking = ");
  Serial.println(handShaking);


  if (handShaking == true)
  {
    this->tryWriteBytes();
    this->handleClientConnections();
  }
  Serial.println(":::::::Finished radio.handleConnections");
}

// void Radio::handleClientConnections()
// {
//   if (_receiver->failureDetected)
//   {
//     Serial.println("RF24 Failure Detected. Re-running the setup.");
//     this->setup();
//   }
//
//   if (this->tryReadBytes()) { // Populates the responsePacket.
//      console->processCommand(responsePacket, &requestPacket); // populate the requestPacket
//   }
//
//   this->tryWriteBytes(requestPacket);
//
// }

void Radio::findChannel()
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

void Radio::setChannel(byte channel)
{
  _receiver->setChannel(channel);
  channelSelected = channel;
}

void Radio::changeChannel()
{
  Serial.println("changeChannel");
  //Bug need to call findChannel() 2 times for it to work. investigate.
  findChannel();
}

void Radio::generateRandomAddress()
{
  randomSeed(analogRead(A0));
  foundAddresses[0] = random(0, 255);
  foundAddresses[1] = random(0, 255);
  foundAddresses[2] = random(0, 255);
  foundAddresses[3] = random(0, 255);
  foundAddresses[4] = random(0, 255);
}

void Radio::setAddress(byte address[])
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

//TODO:: In the future we need to manage the addresses per device.
// Pipes are setting the addresses.
void Radio::openPipes()
{
  _receiver->openReadingPipe(1, currentAddresses[0]);
  _receiver->openWritingPipe(currentAddresses[1]);
  _receiver->startListening();
}

void Radio::resetConnection()
{
  Serial.println("START RESET RADIO CONN");
  setChannel(channelDefault);
  setAddress(defaultAddresses[0]);
  generateRandomAddress();
  openPipes();
  Serial.println("END RESET RADIO CONN");
}


bool Radio::tryReadBytes()
{
  bool timeout = false;
  bool success = false;
  long started_waiting_at = millis();

  while (!_receiver->available())
  {
    yield();
    if(millis() - started_waiting_at > _TIMEOUT_READ)
    {
      timeout = true;
      Serial.println("Radio Read timeout reached");
      break;
    }
  }

  if (!timeout)
  {
    success = true;
    //controllerConected = true;
    // metroController->reset();
    _receiver->read(&responsePacket, packetSize); // populate teh responsePacket
    lastPacketId = responsePacket.Id;
    yield();
  }
  else
  {
    Serial.println("Reading from Radio Client failes. Reason: timeout");
  }
  return success;
}


// TODO:: We need to assmble the requestPacket somwhere and pass it in.
bool Radio::tryWriteBytes()
{
  bool success = false;
  //this->requestPacket = requestPacket; // Copy requestPacket so we always have the previous requestPacket.
  //Don't listen while writing
  _receiver->stopListening();
  this->printRequestPacket();
  if(_receiver->write(&requestPacket, packetSize, 1))
  {
    sendCount++;
    success = true;
  }
  else
  {
    Serial.println("Radio Failed to Write Bytes");
  }

  // This function should be called as soon as transmission is finished to drop the radio back to STANDBY-I mode
  _receiver->txStandBy();
  _receiver->startListening();
  yield();
  return success;
}






void Radio::printRequestPacket()
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

void Radio::printResponsePacket()
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

void Radio::printAddresses()
{
  Serial.println();
  Serial.print("READ ADDRESS :: ");
  Serial.print(currentAddresses[0][0]);
  Serial.print(" | ");
  Serial.print(currentAddresses[0][1]);
  Serial.print(" | ");
  Serial.print(currentAddresses[0][2]);
  Serial.print(" | ");
  Serial.print(currentAddresses[0][3]);
  Serial.print(" | ");
  Serial.print(currentAddresses[0][4]);
  Serial.print(" | ");

  Serial.println();
  Serial.print("Write ADDRESS :: ");
  Serial.print(currentAddresses[1][0]);
  Serial.print(" | ");
  Serial.print(currentAddresses[1][1]);
  Serial.print(" | ");
  Serial.print(currentAddresses[1][2]);
  Serial.print(" | ");
  Serial.print(currentAddresses[1][3]);
  Serial.print(" | ");
  Serial.print(currentAddresses[1][4]);
  Serial.print(" | ");
  Serial.println();

}
