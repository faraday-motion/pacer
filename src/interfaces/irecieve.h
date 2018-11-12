#ifndef IRECIEVE_H
#define IRECIEVE_H

#include <Arduino.h>

class IRecieve {
   public:
      virtual void recieve(byte command, byte value) = 0;
};
#endif
