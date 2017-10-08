#ifndef RadioHandshake_h
#define RadioHandshake_h
#include "Radio.h"
#include <Metro.h>
#include "RadioPacket.h"
#include "components/Device/AbstractDevice.h"

class RadioHandshake {

private:
  Radio* _radio;
  // Timer Constants
  unsigned int _LOST_CONNECTION = 500;

  // Timers
  Metro* handshakeTimeout;

  // Packets
  RadioPacket requestPacket;
  RadioPacket responsePacket;

  // Addresses
  byte foundAddresses[6];

  // Channel
  byte foundChannel = 255;

  void generateRandomAddress();
  bool deviceHasValidId(RadioPacket* idPacket);
  void clearPayload(RadioPacket* packet);
  void findChannel();
  void intercept();
  void initHandshake();
public:
  AbstractDevice pendingDevice;

  RadioHandshake(Radio* radio);

  // Main loop
  void handleHandshake();
  // Process Response. Take action if required and populate request.
  void processResponse();

  // Debug
  void printDeviceCredentials(AbstractDevice d);

};

#endif
