#include <Arduino.h>
#include "Radio.h"
#include "components/Utility/Log.h"



Radio::Radio() :
defaultAddresses{"FM01R", "FM01W"},  // initialize defaultAddresses
currentAddresses{"FM01R", "FM01W"},  // initialize currentAddresses
foundAddresses{"FM000"}              // initialize foundAddresses
{
  this->_receiver = new RF24(4,15); // TODO:: These are environment pin configurations. Should get it out of the .ini file.
}

void Radio::setup()
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
  generateRandomAddress();
  openPipes();
  Serial.println("Finished Setting Up Nunchuck Controller");
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
