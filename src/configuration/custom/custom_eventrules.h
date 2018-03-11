#ifndef CUSTOM_EVENTRULES_H
#define CUSTOM_EVENTRULES_H

#include <Arduino.h>
#include "../default/default_eventrules.h"
#include "../default/configuration.h"

class Custom_eventrules : public Default_eventrules{
private:
public:
  Custom_eventrules() : Default_eventrules() {
  }
  /*
  void moduleEvent(Modulebase* sender, byte eventId)
  {

  }
  */
};

#endif
