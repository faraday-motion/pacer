#ifndef Radio_h
#define Radio_h

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Metro.h>
#include "RadioDevice.h"
#include "components/Communication/Console.h"
#include "components/Controller/NunchuckController/ControllerPacket.h"

class Radio {
private:
  ControllerPacket responsePacket;
  ControllerPacket requestPacket;

  // Intervals
  byte _TIMEOUT_READ = 50;

  // Togglers and watchers
  byte lastPacketId = 0;
public:
  RF24* _receiver;
  Radio();
  void readWrite();
  void processResponse();
  void setup();
  bool handleClientConnections();

  bool changeDevice(RadioDevice device);
  // Packet Handler //TODO:: See how can we abstract this. Basically
  byte packetSize   = 7;
  byte sendCount    = 0;

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
  bool handShaking = false;
  byte handShakeCount = 0;
  byte handShakeMaxAttempts = 30;
  bool handShakeSucceeded = false;


  // Connection
  void findChannel();
  void setChannel(byte channel);
  void changeChannel();
  void generateRandomAddress();
  void setAddress(byte address[]);
  void openPipes();
  void resetConnection();
  void initPackets();

  // Read/Write Packets

  bool tryReadBytes(ControllerPacket* response);
  bool tryWriteBytes(ControllerPacket* request);

  RadioDevice pendingDevice;
  void clearPendingDevice();
  void listenToRegisteredRadioDevice(RadioDevice device);

  // Debug
  void printRequestPacket();
  void printResponsePacket();
  void printAddresses();
};


#endif
