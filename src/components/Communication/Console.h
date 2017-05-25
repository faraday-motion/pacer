#ifndef Console_h
#define Console_h
#include "components/Controller/NunchuckController/ControllerPacket.h"

class Console {
public:
  Console();
  void processCommand(ControllerPacket packet, ControllerPacket* requestPacket);
};



#endif
