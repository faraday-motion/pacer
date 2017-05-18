#ifndef NunchuckController_h
#define NunchuckController_h

#include "../AbstractController.h"
#include "components/Connection/Radio.h"

// #include <SPI.h>
// #include <nRF24L01.h>
// #include <RF24.h>
#include "ControllerPacket.h"
#include <Metro.h>

class NunchuckController : public AbstractController
{
private:


  Radio* radio;
  /**
    Intervals
  */
  byte _TIMEOUT_READ = 50;
  byte _SIGNAL_CHECK_INTERVAL = 250;
  byte _READ_INTERVAL = 10;

  /**
    Metro Timers
  */
  Metro* metroCommunication;
  Metro* metroController;
  Metro* metroHasController;
  Metro* metroChannelChange;


  byte transmitterId[6];



  // Packet Handling
  ControllerPacket responsePacket;
  ControllerPacket requestPacket;
  byte sendCount    = 0;
  unsigned long receiveCounter = 0;
  byte packetSize   = 7;
  byte lastPacketId = 0;
  byte sendCommand  = 1;

  bool controllerConnected = false;
  bool controllerVerified  = false;



  // Packet Handling
  bool isNewOrKnownController();
  bool tryReadBytes();
  bool tryWriteBytes();

  // Debug
  void printRequestPacket();
  void printResponsePacket();
  void printAddresses();


public:

  NunchuckController(ConfigController* configController, byte controllerType, byte controllerId);
  void read();
  void write();
  void setup();
  void handleController();

};

#endif
