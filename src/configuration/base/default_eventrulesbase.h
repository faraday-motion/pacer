#ifndef DEFAULT_EVENTRULESBASE_H
#define DEFAULT_EVENTRULESBASE_H

#include <Arduino.h>
#include "../../interfaces/interfaces.hpp"

class Default_eventrulesbase {
public:
  Default_eventrulesbase() {

  }

  virtual void moduleEvent(IModule * sender, byte eventId);
  virtual void setFMV(IFMV * fmv);
};

#endif
