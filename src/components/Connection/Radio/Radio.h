#ifndef Radio_h
#define Radio_h

#include <SPI.h>
#include <RF24.h>
#include <Metro.h>
#include <nRF24L01.h>
#include "components/Device/AbstractDevice.h"
#include "RadioPacket.h"

class Radio {
private:

  // RF24 Object Pointer
  RF24* _receiver;

  // Metro Timers
  Metro* connectionLostTimer;

  // Connection defaults, flags and inits.
  byte currentAddresses[2][6];

  // Channel
  byte channelMax = 125;
  byte channelMin = 100;
  byte currentChannel = 100;

  byte connectionStrength = 0;
  bool handShaking = false;
  byte packetSize   = 7;
  byte sendCount    = 0;

  // Intervals
  byte _TIMEOUT_READ = 50;
  unsigned int _LOST_CONNECTION = 500;

  // Togglers and watchers
  byte lastPacketId = 0;

  // Connection
  void openPipes();
  void resetConnection();

public:
  // Public defaults
  byte channelDefault  = 100;
  byte defaultAddress[5];

  Radio();
  void setup();

  byte findChannel();

  void setAddress(byte address[]);
  void setChannel(byte channel);

  // Read/Write Packets
  bool tryReadBytes(RadioPacket* response);
  bool tryWriteBytes(RadioPacket* request);

  // Debug

  void printAddresses();
  void printPacket(RadioPacket* packet, bool isRequest = true);
};


#endif
