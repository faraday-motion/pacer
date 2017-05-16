#ifndef NunchuckController_h
#define NunchuckController_h

#include "../AbstractController.h"
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "ControllerPacket.h"
#include <Metro.h>

class NunchuckController : public AbstractController
{
private:
  /**
    Communication Packets
  */
  ControllerPacket responsePacket;
  ControllerPacket requestPacket;

  /**
    Metro Timers
  */
  Metro* metroCommunication;
  Metro* metroController;
  Metro* metroHasController;
  Metro* metroChannelChange;

  //Note the address is mirrored with the other transiever
  byte transmitterId[6];
  byte defaultAddresses[2][6];
  byte currentAddresses[2][6];
  byte foundAddresses[6];
  byte channelDefault = 100;
  byte channelSelected = 100;
  byte channelFound = 255;
  byte channelMax = 125;
  byte channelMin = 100;
  bool controllerConnected = false;
  bool controllerVerified = false;
  byte sendCommand  = 1;
  byte packetSize   = 7;
  byte lastPacketId = 0;
  byte sendCount    = 0;
  unsigned long receiveCounter = 0;
  byte connectionStrength = 0;

  byte _TIMEOUT_READ = 50;
  byte _SIGNAL_CHECK_INTERVAL = 250;
  byte _READ_INTERVAL = 10;

  void findChannel();
  void setChannel(byte channel);
  void generateRandomAddress();
  void setAddress(byte address[]);
  void openPipes();
  bool isNewOrKnownController();
  void changeChannel();
  bool tryReadBytes();
  bool tryWriteBytes();
  void resetConnection();

  // Debug
  void printRequestPacket();
  void printResponsePacket();


public:

  RF24* _receiver;
  NunchuckController(ConfigController* configController, MotorController* motorController, byte controllerType, byte controllerId);
  void read();
  void write();
  void setup();
  void listenToController();

};

#endif
