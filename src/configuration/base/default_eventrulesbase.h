#ifndef DEFAULT_EVENTRULESBASE_H
#define DEFAULT_EVENTRULESBASE_H
#include <Arduino.h>
#include "../../modules/base/modulebase.h"

class Default_eventrulesbase {
public:
  Default_eventrulesbase() {

  }

  virtual void moduleEvent(Modulebase* sender, byte eventId);
};

#endif
