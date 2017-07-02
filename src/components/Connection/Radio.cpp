#include <Arduino.h>
#include "Radio.h"
#include "components/Utility/Log.h"

Radio::Radio() :
defaultAddresses{"FM01R", "FM01W"},  // initialize defaultAddresses
currentAddresses{"FM01R", "FM01W"},  // initialize currentAddresses
foundAddresses{"FM000"}             // initialize foundAddresses
{
  this->_receiver = new RF24(4,15); // TODO:: These are environment pin configurations. Should get it out of the .ini file.
}

void Radio::setup()
{
  Serial.println("Setting Up Radio Connection");
  // Setup Metro Intervals
  this->connectionLostTimer = new Metro(_LOST_CONNECTION);

  // Setup the NRF24
  _receiver->begin();
  _receiver->setAutoAck(false);
  _receiver->setRetries(0, 0);
  //_receiver->setDataRate(RF24_250KBPS);
  //_receiver->setPALevel(RF24_1MBPS);
  _receiver->setPALevel(RF24_1MBPS); // Uncomment for more power
  _receiver->setPayloadSize(packetSize);
   this->setChannel(channelDefault); // we also set the currentChannel member here.
  _receiver->failureDetected = 0;
  //_receiver->printDetails();
  delay(50);
  findChannel();
  delay(50);
  setAddress(defaultAddresses[0]);
  delay(50);
  generateRandomAddress();
  openPipes();

  this->initPackets();
  this->clearPendingDevice();
  //this->printRequestPacket();
  //this->printResponsePacket();
  Serial.println("Finished Setting Up Radio Connection");
  yield();
}


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

void Radio::processResponse()
{

  if (responsePacket.Command == 5)
  {
    handShaking = true;
    //We have a name packet
    Serial.println("Name recived");
    // Setting the pendingDevice ID.
    pendingDevice.id[0] = responsePacket.Value1 ;
    pendingDevice.id[1] = responsePacket.Value2 ;
    pendingDevice.id[2] = responsePacket.Value3 ;
    pendingDevice.id[3] = responsePacket.Value4 ;
    pendingDevice.id[4] = responsePacket.Value5 ;
    pendingDevice.type  = 2; // nunchuck.

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

    // Storing the address of the pending device.
    pendingDevice.address[0] = this->foundAddresses[0];
    pendingDevice.address[1] = this->foundAddresses[1];
    pendingDevice.address[2] = this->foundAddresses[2];
    pendingDevice.address[3] = this->foundAddresses[3];
    pendingDevice.address[4] = this->foundAddresses[4];

    this->openPipes();

    //Send a request for the channel change
    requestPacket.Command = 20;
    requestPacket.Value1  = channelFound;
  }
  else if (responsePacket.Command == 20)
  {
    //We have a channel change request
    Serial.println("Channel change");
    this->changeChannel();
    requestPacket.Command = 20; // moved from changeChannel() for abstracting the Radio
  }
  else if (responsePacket.Command == 25)
  {
    //We have a channel change confirmation
    Serial.println("Channel change confirmed");
    // Setting the channel of the pendingDevice.
    pendingDevice.channel = this->channelFound;
    this->setChannel(this->channelFound);

    requestPacket.Command = 40;
  }
  else if (responsePacket.Command == 44)
  {
    Serial.println("Radio Device handShaking finished. The divice is IDLE");
    this->validatePendingDevice(); // Decides what to do with the pending Devices.
    //pendingDevice.pending = true; // flag handshake has finished.

    handShakeSucceeded = true; //TODO:: Remove
    this->initPackets();
    this->requestPacket.Command = 0; //Quick hack. To assure that we don't send a name request to an already pending device.
    // TODO:: Think that the responsePacket might need to be nullified or smth.
  }

}

void Radio::validatePendingDevice()
{
  if (pendingDevice.id[0] == 0 || pendingDevice.id[1] == 0 || pendingDevice.id[2] == 0 || pendingDevice.id[3] == 0 || pendingDevice.id[4] == 0)
  {
    pendingDevice.pending = false;
  } else {
    pendingDevice.pending = true;
    handShaking = false; // flag that the handShaking was finished.
  }

  // TODO:: improve the validator
}


void Radio::readWrite()
{
  if (_receiver->failureDetected)
  {
    //Serial.println("RF24 Failure Detected. Re-running the setup.");
    this->setup();
  }
  this->printAddresses();
  //Serial.print("Channel: ");
  //Serial.println(channelSelected);

  if (this->tryReadBytes(&responsePacket)) { // Populates the responsePacket.
     this->printResponsePacket();
     delay(5);
     this->processResponse(); // populate the requestPacket
  }
  yield();
  delay(5);
  this->printRequestPacket();
  this->tryWriteBytes(&requestPacket);
}

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

void Radio::setChannel(byte channel)
{
  _receiver->setChannel(channel);
  channelSelected = channel;
}

void Radio::changeChannel()
{
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
  foundAddresses[4] = 0; // This is a slot for read/write flag
  //foundAddresses[4] = random(0, 255);
}

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
  //Serial.println("START RESET RADIO CONN");
  setChannel(channelDefault);
  setAddress(defaultAddresses[0]);
  generateRandomAddress();
  openPipes();
  delay(1);
  //this->printAddresses();
  //Serial.println("END RESET RADIO CONN");
}

bool Radio::changeDevice(RadioDevice device)
{
  //Serial.println("Switching Devices");
  setChannel(device.channel);
  setAddress(device.address);
  openPipes();
  delayMicroseconds(500);
  //Serial.println("Finished Device Switch.");

  //Serial.println("Listening To Device ::");
  //this->printDeviceCredentials(device);
}


bool Radio::tryReadBytes(ControllerPacket* response)
{
  bool timeout = false;
  bool success = false;
  long started_waiting_at = millis();
  //Serial.println("tryReadBytes() :::");
  //this->printAddresses();
  if (connectionLostTimer->check() == 1)
  {
    //Serial.println("Connection has been lost");
    Log::Instance()->write("EVENT: Connection Lost, tryReadBytes()");
    this->resetConnection();
    this->initPackets();
    this->clearPendingDevice();
  }
  while (!_receiver->available())
  {
    yield();
    if(millis() - started_waiting_at > _TIMEOUT_READ)
    {
      timeout = true;
      //Serial.println("Radio Read timeout reached. On tryReadBytes()");
      break;
    }
  }

  if (!timeout)
  {
    success = true;
    //Serial.println("connectionLostTimer is reseted");
    connectionLostTimer->reset();
    _receiver->read(response, packetSize); // populate the responsePacket
    lastPacketId = response->Id;
    yield();
  }
  else
  {
    //Serial.println("Reading from Radio Client failes. Reason: timeout");
  }
  return success;
}


bool Radio::tryWriteBytes(ControllerPacket* request)
{
  bool success = false;
  //this->requestPacket = requestPacket; // Copy requestPacket so we always have the previous requestPacket.

  //Don't listen while writing
  _receiver->stopListening();

  if(_receiver->write(request, packetSize, 1))
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


void Radio::clearPendingDevice()
{
  //Serial.println("Clearing Radio Pending Device");
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
  //this->printDeviceCredentials(pendingDevice);
}



void Radio::initPackets()
{

  //Serial.println("Initializing Packets");
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
  //this->printResponsePacket();
  //this->printRequestPacket();
}


// DEBUG

void Radio::printDeviceCredentials(RadioDevice d)
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
