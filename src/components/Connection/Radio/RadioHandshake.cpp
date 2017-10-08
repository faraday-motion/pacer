#include "RadioHandshake.h"
#include "components/Utility/Log.h"
#include "Commands.h"
RadioHandshake::RadioHandshake(Radio* radio) :
foundAddresses{"FM000"}              // initialize foundAddresses
{
  this->_radio = radio;
  this->handshakeTimeout = new Metro(_LOST_CONNECTION);
  this->handshakeTimeout->reset();

  this->generateRandomAddress();
  this->foundChannel = this->_radio->findChannel();

  this->initHandshake();
}


// Main loop for read and write.
void RadioHandshake::handleHandshake()
{

  if (this->handshakeTimeout->check() == 1) {
    Log::Logger()->write(Log::Level::WARNING, "WARNING:: RadioHandhake timedout.");
    this->initHandshake();
    this->clearPayload(&requestPacket);
    this->clearPayload(&responsePacket);
    return;
  }

  // Switch the radio to the pendingDevice address and channel.
  this->_radio->setChannel(pendingDevice.channel);
  this->_radio->setAddress(pendingDevice.address);
  delay(1);

  if ( this->_radio->tryReadBytes(&responsePacket) ) // populate the responsePacket.
  {
    delay(5);
    this->handshakeTimeout->reset();
    this->intercept();
    this->processResponse();
  }

  this->_radio->tryWriteBytes(&requestPacket); // poplate requestPacket.
}

// Interceptor to break communication loops in case of action required.
void RadioHandshake::intercept()
{
  // Intercept address change.
  if (responsePacket.Command == CONFIRM_CHANGE_ADDRESS) {
    this->_radio->setAddress(foundAddresses);
  }
  // Intercept channel change.
  if (responsePacket.Command == CONFIRM_CHANGE_CHANNEL) {
    this->_radio->setChannel(foundChannel);
  }

  // Intercept succesful registration
  if (responsePacket.Command == CONFIRM_WAIT_REGISTER && pendingDevice.isRegisterd == true) {
    requestPacket.Command = SET_MODE_IDLE;
  }

  // Redundant interceptor for checking if an already registered device has reconnected.
  if (responsePacket.Command == CONFIRM_SET_MODE_ACTIVE && pendingDevice.isRegisterd == true) {
    Log::Logger()->write(Log::Level::DEBUG, "Redundante handshake finish check closed the handshake");
    this->initHandshake();
  }

}

// Proess response and assemble new request.
void RadioHandshake::processResponse()
{
  switch (responsePacket.Command)
  {
    // Name Received from pending device.
    case SAVE_NAME:
      Log::Logger()->write(Log::Level::DEBUG, "SAVE_NAME");
      if (this->deviceHasValidId(&responsePacket))
      {
        // Setting the pendingDevice ID.
        pendingDevice.id[0] = responsePacket.Value1 ;
        pendingDevice.id[1] = responsePacket.Value2 ;
        pendingDevice.id[2] = responsePacket.Value3 ;
        pendingDevice.id[3] = responsePacket.Value4 ;
        pendingDevice.id[4] = responsePacket.Value5 ;
        pendingDevice.type  = 2; // TODO:: This should come in from the connected device.
        pendingDevice.enabled = true; // TODO:: Figure out where can we check if it shoudl be enabled.

        //Assemble Request Packet and request address change
        requestPacket.Command = SAVE_ADDRESS;
        requestPacket.Value1  = this->foundAddresses[0];
        requestPacket.Value2  = this->foundAddresses[1];
        requestPacket.Value3  = this->foundAddresses[2];
        requestPacket.Value4  = this->foundAddresses[3];
        requestPacket.Value5  = this->foundAddresses[4];
      }
      break;

    case CONFIRM_SAVE_ADDRESS:
      Log::Logger()->write(Log::Level::DEBUG, "CONFIRM_SAVE_ADDRESS");
      if (responsePacket.Value1 == this->foundAddresses[0] &&
          responsePacket.Value2 == this->foundAddresses[1] &&
          responsePacket.Value3 == this->foundAddresses[2] &&
          responsePacket.Value4 == this->foundAddresses[3] &&
          responsePacket.Value5 == this->foundAddresses[4]) {
        Log::Logger()->write(Log::Level::DEBUG, "ADDRESS VALIDATED.");
        requestPacket.Command = CHANGE_ADDRESS;
        this->clearPayload(&requestPacket);
      }
      else {
        Log::Logger()->write(Log::Level::DEBUG, "ERROR:: ADDRESS NOT VALIDATED.");
        // Keeping the previous requestPacket with command SAVE_ADDRESS.
      }
      break;

    case CONFIRM_CHANGE_ADDRESS:
      Log::Logger()->write(Log::Level::DEBUG, "CONFIRM_SAVE_ADDRESS");
      // Address change happens in the intercept in the handleHandshake method.

      pendingDevice.address[0] = foundAddresses[0];
      pendingDevice.address[1] = foundAddresses[1];
      pendingDevice.address[2] = foundAddresses[2];
      pendingDevice.address[3] = foundAddresses[3];
      pendingDevice.address[4] = foundAddresses[4];

      requestPacket.Command = SAVE_CAHNNEL;
      requestPacket.Value1  = foundChannel;
      break;

    case CONFIRM_SAVE_CHANNEL:
      Log::Logger()->write(Log::Level::DEBUG, "CONFIRM_SAVE_CHANNEL");
      if (responsePacket.Value1 == foundChannel)
      {
        Log::Logger()->write(Log::Level::DEBUG, "CHANNEL VALIDATED.");
        requestPacket.Command = CHANGE_CHANNEL;
        this->clearPayload(&requestPacket);
      } else {
        Log::Logger()->write(Log::Level::DEBUG, "ERROR:: CHANNEL NOT VALIDATED.");
        // Keeping the previous requestPacket with command SAVE_CHANNEL.
      }
      break;

    case CONFIRM_CHANGE_CHANNEL:
      Log::Logger()->write(Log::Level::DEBUG, "CONFIRM_CHANGE_CHANNEL");
      // Channel change happens in the intercept in the handleHandshake method
      pendingDevice.channel = foundChannel;

      requestPacket.Command = WAIT_REGISTER;
      this->clearPayload(&requestPacket);
      break;

    case CONFIRM_WAIT_REGISTER:
      Log::Logger()->write(Log::Level::DEBUG, "CONFIRM_WAIT_REGISTER");
      // NOTE:: Interceptor is setting the new command.
      // Flag the peding device is waiting to be registerd.
      pendingDevice.pending = true;
      //TODO:: Handle failure of registration.

      break;
    case CONFIRM_SET_MODE_IDLE:
        Log::Logger()->write(Log::Level::DEBUG, "CONFIRM_SET_MODE_IDLE");
        Log::Logger()->write(Log::Level::INFO, "Handshake with radio device finished.");
        this->initHandshake(); //
        // Kepp setting the idle mode.
        break;
  }
}

/**
  Method that validates that the device that is trying to connect is valid,
  as it has all the required parameters (id, address, type).
  Only once validated is the devices passed in to be registered.
*/
bool RadioHandshake::deviceHasValidId(RadioPacket* packet)
{
  // TODO:: have a proper validation method on the abstract device class tha we can call.
  if (packet->Value1 == 0 ||
      packet->Value2 == 0 ||
      packet->Value3 == 0 ||
      packet->Value4 == 0 ||
      packet->Value5 == 0) {
    Log::Logger()->write(Log::Level::WARNING, "Device failed ID validation.");
    return false;
  }
  return true;
}


// Clears the payload so we don't send it with a wrong command.
void RadioHandshake::clearPayload(RadioPacket* packet)
{
  packet->Value1 = 0;
  packet->Value2 = 0;
  packet->Value3 = 0;
  packet->Value4 = 0;
  packet->Value5 = 0;
}

void RadioHandshake::initHandshake()
{
  // Init Request.
  requestPacket.Command = 1;
  this->clearPayload(&requestPacket);

  // Init Request.
  responsePacket.Command = 0;
  this->clearPayload(&responsePacket);

  // Init channel and address
  pendingDevice.channel    = this->_radio->channelDefault;
  pendingDevice.address[0] = this->_radio->defaultAddress[0];
  pendingDevice.address[1] = this->_radio->defaultAddress[1];
  pendingDevice.address[2] = this->_radio->defaultAddress[2];
  pendingDevice.address[3] = this->_radio->defaultAddress[3];
  pendingDevice.address[4] = this->_radio->defaultAddress[4];
  pendingDevice.pending = false;
  pendingDevice.isRegisterd = false;

  // Set defaultAddress
  this->_radio->setAddress(this->_radio->defaultAddress);
  this->_radio->setChannel(this->_radio->channelDefault);
}

/**
  Generates a new address block to be used for writing and reading.
*/
void RadioHandshake::generateRandomAddress()
{
  randomSeed(analogRead(A0));
  foundAddresses[0] = random(0, 255);
  foundAddresses[1] = random(0, 255);
  foundAddresses[2] = random(0, 255);
  foundAddresses[3] = random(0, 255);
  foundAddresses[4] = 0; // This is a slot for read/write flag
}

/*********
  Set of print methods used for debugging purposes.
*********/

void RadioHandshake::printDeviceCredentials(AbstractDevice d)
{
  Serial.println("::Pending Connection Device::");
  Serial.print("Id :: ");
  Serial.print(d.id[0]);
  Serial.print(" ");
  Serial.print(d.id[1]);
  Serial.print(" ");
  Serial.print(d.id[2]);
  Serial.print(" ");
  Serial.print(d.id[3]);
  Serial.print(" ");
  Serial.println(d.id[4]);

  Serial.print("Read/Write Address: ");
  Serial.print(d.address[0]);
  Serial.print(" ");
  Serial.print(d.address[1]);
  Serial.print(" ");
  Serial.print(d.address[2]);
  Serial.print(" ");
  Serial.print(d.address[3]);
  Serial.print(" ");
  Serial.println(d.address[4]);

  Serial.print("Type: ");
  Serial.println(d.type);

  Serial.print("Channel: ");
  Serial.println(d.channel);

  Serial.print("Is Pending: ");
  Serial.println(d.pending);

  Serial.print("Is Registered: ");
  Serial.println(d.isRegisterd);
}
