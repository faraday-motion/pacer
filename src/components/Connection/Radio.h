#ifndef Radio_h
#define Radio_h

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Metro.h>

class Radio {
public:
  RF24* _receiver;
  Radio();
  void setup();
  // Packet Handler //TODO:: See how can we abstract this. Basically
  byte packetSize   = 7;

  // Connection
  byte defaultAddresses[2][6];
  byte currentAddresses[2][6];
  byte foundAddresses[6];
  byte channelDefault  = 100;
  byte channelSelected = 100;
  byte channelFound = 255;
  byte channelMax = 125;
  byte channelMin = 100;
  byte connectionStrength = 0;

  // Connection
  void findChannel();
  void setChannel(byte channel);
  void changeChannel();

  void generateRandomAddress();
  void setAddress(byte address[]);

  void openPipes();

  void resetConnection();

};


#endif
