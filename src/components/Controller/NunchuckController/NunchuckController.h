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

  void processResponse();

public:

  NunchuckController(Radio* radio, AbstractDevice device);

  bool handleController();
  bool enable();
  bool disable();
};

#endif
