#include <Arduino.h>
#include "Radio.h"
#include "components/Utility/Log.h"

Radio::Radio() :
defaultAddresses{"FM01R", "FM01W"},  // initialize defaultAddresses
currentAddresses{"FM01R", "FM01W"},  // initialize currentAddresses
foundAddresses{"FM000"}             // initialize foundAddresses
{
  this->_receiver = new RF24(10,15); // TODO:: These are environment pin configurations. Should get it out of the .ini file.
  //this->_receiver = new RF24(4,15); // TODO:: These are environment pin configurations. Should get it out of the .ini file.
}

void Radio::setup()
{
  Log::Logger()->write(Log::Level::INFO, "Setting Up Radio Connection");

  // Setup Metro Intervals
  this->connectionLostTimer = new Metro(_LOST_CONNECTION);

  // Setup the NRF24
  _receiver->begin();
  _receiver->setAutoAck(false);
  _receiver->setRetries(0, 0);
  //_receiver->setDataRate(RF24_250KBPS);
  //_receiver->setPALevel(RF24_1MBPS);
  //_receiver->setPALevel(RF24_1MBPS); // Uncomment for more power
  _receiver->setPALevel(RF24_PA_MAX);
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

  this->initPackets();
  this->clearPendingDevice();
  Log::Logger()->write(Log::Level::INFO, "Finished Setting Up Radio Connection");
  yield();
}


/**
  Method that scans the default channel & address for new
  incoming device attempting to connect.

  This method manages the handShaking process.
*/
bool Radio::handleClientConnections()
{
  // Set defaut address and channel for scanning.
  if (handShaking == false)
  {
    this->resetConnection(); // checking for devices on default device address.
  }

  //Quick hack. To assure that we don't send a name request to an already pending device.
  if (requestPacket.Command == 0)
  {
    requestPacket.Command = 5;
  }
  // Resuming communication with the pending Device.
  this->changeDevice(pendingDevice);

  // Read packets, assemble packet and write it.
  this->readWrite();


  return pendingDevice.pending; // corner cases when we send true by accident?
}


/**
  Method that processes a responsePacket and assembles a new requestPacket.
*/
void Radio::processResponse()
{

  if (responsePacket.Command == 5)
  {
    // Handshake initiated
    handShaking = true;

    //We have a name packet
    Log::Logger()->write(Log::Level::DEBUG, "Name recived");

    // Setting the pendingDevice ID.
    pendingDevice.id[0] = responsePacket.Value1 ;
    pendingDevice.id[1] = responsePacket.Value2 ;
    pendingDevice.id[2] = responsePacket.Value3 ;
    pendingDevice.id[3] = responsePacket.Value4 ;
    pendingDevice.id[4] = responsePacket.Value5 ;
    pendingDevice.type  = 2; // nunchuck. // TODO:: This should come in from the connected device.

    //Assemble Request Packet and request address change
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
    Log::Logger()->write(Log::Level::DEBUG, "Address change confirmed");
    this->setAddress(this->foundAddresses);

    // Storing the address of the pending device.
    pendingDevice.address[0] = this->foundAddresses[0];
    pendingDevice.address[1] = this->foundAddresses[1];
    pendingDevice.address[2] = this->foundAddresses[2];
    pendingDevice.address[3] = this->foundAddresses[3];
    pendingDevice.address[4] = this->foundAddresses[4];

    // Set the current read/write addresses.
    this->openPipes();

    //Send a request for the channel change
    requestPacket.Command = 20;
    requestPacket.Value1  = channelFound;
  }

  else if (responsePacket.Command == 20)
  {
    //We have a channel change request
    Log::Logger()->write(Log::Level::DEBUG, "Channel Change");

    // Find and set the new channel.
    this->findChannel();

    // Let the listeing devices that we are still looking for a channel.
    requestPacket.Command = 20;
  }
  else if (responsePacket.Command == 25)
  {
    //We have a channel change confirmation
    Log::Logger()->write(Log::Level::DEBUG, "Channel change confirmed");

    // Setting the channel of the pendingDevice.
    pendingDevice.channel = this->channelFound;
    this->setChannel(this->channelFound);

    // Set idle command.
    requestPacket.Command = 40;
  }
  else if (responsePacket.Command == 44)
  {
    Log::Logger()->write(Log::Level::DEBUG, "Radio Device handShaking finished.");

    // Validates that the connected device is valid.
    this->validatePendingDevice();

    // Reset the request/responsePacket
    this->initPackets();
    this->requestPacket.Command = 0; //Quick hack. To assure that we don't send a name request to an already pending device.
  }

}

/**
  Method that validates that the device that is trying to connect is valid,
  as it has all the required parameters (id, address, type).
  Only once validated is the devices passed in to be registered.
*/
void Radio::validatePendingDevice()
{
  // TODO:: have a proper validation method on the abstract device class tha we can call.
  if (pendingDevice.id[0] == 0 || pendingDevice.id[1] == 0 || pendingDevice.id[2] == 0 || pendingDevice.id[3] == 0 || pendingDevice.id[4] == 0)
  {
    pendingDevice.pending = false;
  } else {
    pendingDevice.pending = true;
    handShaking = false; // flag that the handShaking was finished.
  }
}

/**
  Method that reads and processes a responsePacket,
  and then assembles and write a new requestPacket.
*/
void Radio::readWrite()
{
  if (_receiver->failureDetected)
  {
    Log::Logger()->write(Log::Level::WARNING, "RF24 Failure Detected. Re-running the setup.");
    this->setup();
  }

  if (this->tryReadBytes(&responsePacket)) { // Populates the responsePacket.
     //this->printResponsePacket();
     delay(5);
     this->processResponse(); // populate the requestPacket
  }

  // Breathing space
  yield();
  delay(5);

  // Write the requestPacket
  this->tryWriteBytes(&requestPacket);
}


/**
   Finds the less crowded channel and sets it as the currentChannel.
*/
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
  } // end for readWrite.

  //If a good channel is not found, continue with the default one
  if (channelFound == 255)
    channelFound = channelDefault;

  _receiver->stopListening();
  setChannel(currentChannel);
}


/**
   Sets a radio channel.
   @parameter byte channel.
*/
void Radio::setChannel(byte channel)
{
  _receiver->setChannel(channel);
  channelSelected = channel;
}

/**
  Generates a new address block to be used for writing and reading.
*/
void Radio::generateRandomAddress()
{
  randomSeed(analogRead(A0));
  foundAddresses[0] = random(0, 255);
  foundAddresses[1] = random(0, 255);
  foundAddresses[2] = random(0, 255);
  foundAddresses[3] = random(0, 255);
  foundAddresses[4] = 0; // This is a slot for read/write flag
}


/**
   Takes an address block and sets the current read/write addresses.
*/
void Radio::setAddress(byte address[])
{
  // Here we assemble address;
  for (byte i = 0; i < 5; i++)
  {
    currentAddresses[0][i] = address[i];
    currentAddresses[1][i] = address[i];
  }
  currentAddresses[0][4] = 'R'; // Setting read address
  currentAddresses[1][4] = 'W'; // Setting write address
}


/**
   Enable the current read/write addresses.
*/
void Radio::openPipes()
{
  _receiver->openReadingPipe(1, currentAddresses[0]);
  _receiver->openWritingPipe(currentAddresses[1]);
  _receiver->startListening();
}


/**
   Resets the radio connection
   1. Sets Default channel
   2. Sets Default read/write addresses
   3. Generates a new set of random read/write addresses
   4. Enabbles the default read/write addresses
*/
void Radio::resetConnection()
{
  Log::Logger()->write(Log::Level::DEBUG, "START RESET RADIO CONN");
  setChannel(channelDefault);
  setAddress(defaultAddresses[0]);
  generateRandomAddress();
  openPipes();
  delay(1);
}

/**
  Method that changes the device we are communicating with.
*/
void Radio::changeDevice(AbstractDevice device)
{
  Log::Logger()->write(Log::Level::DEBUG, "Switching Devices");
  setChannel(device.channel);
  setAddress(device.address);
  //this->printDeviceCredentials(device);
  openPipes();
  delayMicroseconds(500);
}


/**
   Attempts to read the requestPacket.
   @return bool
*/
bool Radio::tryReadBytes(RadioPacket* response)
{
  // Keep track of performance
  bool timeout = false;
  bool success = false;
  long started_waiting_at = millis();

  // Check if we've been trying to write for too long.
  if (connectionLostTimer->check() == 1)
  {
    Log::Logger()->write(Log::Level::WARNING, "Radio Connection has been lost");
    //Log::Instance()->write("EVENT: Connection Lost, tryReadBytes()");

    // Reset the connection
    this->resetConnection();
    // Clear the respose/requestPackets
    this->initPackets();
    // Forget the device with whom we are failing to handshake
    this->clearPendingDevice();
  }

  // Check if we have available bytes to read.
  while (!_receiver->available())
  {
    yield();
    if(millis() - started_waiting_at > _TIMEOUT_READ)
    {
      timeout = true;
      Log::Logger()->write(Log::Level::WARNING, "Radio Read timeout reached. On tryReadBytes()");
      break;
    }
  }


  // No issues with connection
  if (!timeout)
  {
    success = true;
    connectionLostTimer->reset(); // reset lost connection timer

    // Populate the responsePacket
    _receiver->read(response, packetSize);
    lastPacketId = response->Id; // TODO:: Make use of this.
    yield();
  }
  else
  {
    Log::Logger()->write(Log::Level::WARNING, "Reading from Radio Client failes. Reason: timeout");
  }
  return success;
}


/**
   Attempts to send the requestPacket over.
   @return bool
*/
bool Radio::tryWriteBytes(RadioPacket* request)
{
  bool success = false;

  //Don't listen while writing
  _receiver->stopListening();

  if(_receiver->write(request, packetSize, 1))
  {
    sendCount++;
    success = true;
  }
  else
  {
    Log::Logger()->write(Log::Level::ERR, "Radio Failed to Write Bytes");
  }

  // This function should be called as soon as transmission is finished to drop the radio back to STANDBY-I mode
  _receiver->txStandBy();
  _receiver->startListening();

  yield(); // Breathing space.

  return success;
}


/**
   Clear the parameters of the pending device.
*/
void Radio::clearPendingDevice()
{
  // Setting back to default
  for (byte i = 0; i < 5; i++) {
    pendingDevice.id[i] = 0;
  }
  pendingDevice.address[0] = defaultAddresses[0][0];
  pendingDevice.address[1] = defaultAddresses[0][1];
  pendingDevice.address[2] = defaultAddresses[0][2];
  pendingDevice.address[3] = defaultAddresses[0][3];
  pendingDevice.address[4] = defaultAddresses[0][4];

  pendingDevice.channel = channelDefault;
  pendingDevice.pending = false;
  pendingDevice.type = 0;
}


/**
   Initialize response/requestPackets.
*/
void Radio::initPackets()
{
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
  requestPacket.Command = 1; // default command for seeking.
  requestPacket.Value1  = 0;
  requestPacket.Value2  = 0;
  requestPacket.Value3  = 0;
  requestPacket.Value4  = 0;
  requestPacket.Value5  = 0;
}

/*********
  Set of print methods used for debugging purposes.
*********/

void Radio::printDeviceCredentials(AbstractDevice d)
{
  Serial.print("ID: ");
  Serial.print(d.id[0]);
  Serial.print(" ");
  Serial.print(d.id[1]);
  Serial.print(" ");
  Serial.print(d.id[2]);
  Serial.print(" ");
  Serial.print(d.id[3]);
  Serial.print(" ");
  Serial.println(d.id[4]);

  Serial.print("ADDRESS: ");
  Serial.print(d.address[0]);
  Serial.print(" ");
  Serial.print(d.address[1]);
  Serial.print(" ");
  Serial.print(d.address[2]);
  Serial.print(" ");
  Serial.print(d.address[3]);
  Serial.print(" ");
  Serial.println(d.address[4]);

  Serial.print("CHANNEL: ");
  Serial.println(d.channel);

  Serial.print("PENDING: ");
  Serial.println(d.pending);
}

void Radio::printRequestPacket()
{
  Serial.println("Radio.h Current Request:: ");
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
  Serial.println("Radio.h Packet Response:: ");
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
  Serial.print("Radio.h READ ADDRESS :: ");
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
  Serial.print("Radio.h Write ADDRESS :: ");
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
