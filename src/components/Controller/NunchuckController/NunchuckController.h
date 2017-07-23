#ifndef NunchuckController_h
#define NunchuckController_h

#include <Metro.h>
#include "components/Controller/AbstractController.h"
#include "components/Connection/Radio/Radio.h"
#include "components/Connection/Radio/RadioPacket.h"


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
  AbstractDevice nunchuck;

  // Packet Handling
  RadioPacket responsePacket;
  RadioPacket requestPacket;
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
  void processResponse();

  // Debug
  void printRequestPacket();
  void printResponsePacket();
  void printAddresses();


public:

  NunchuckController(ConfigController* configController, Radio* radio, AbstractDevice device);

  bool handleController();
  bool enable();
  bool disable();



};

#endif
