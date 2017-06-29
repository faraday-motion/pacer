#ifndef NunchuckController_h
#define NunchuckController_h

#include "../AbstractController.h"
#include "components/Connection/Radio.h"
#include "ControllerPacket.h"
#include <Metro.h>

class NunchuckController : public AbstractController
{
private:


  Radio* radio;
  /**
    Intervals
  */
  unsigned int _LOST_CONNECTION = 500;

  /**
    Metro Timers
  */
  Metro* connectionLostTimer;

  /**
    Identification
  */
  RadioDevice nunchuck;

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
  bool tryReadBytes();
  bool tryWriteBytes();

  bool isNewOrKnownController();

  // Debug
  void printRequestPacket();
  void printResponsePacket();
  void printAddresses();


public:

  NunchuckController(ConfigController* configController, Radio* radio, RadioDevice device);
  //~NunchuckController();
  void read();
  void write();
  bool enable();
  bool handleController();
  void processResponse();

};

#endif
