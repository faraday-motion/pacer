#ifndef IWHEEL_H
#define IWHEEL_H

#include <Arduino.h>

class IWheel {
   public:
      virtual bool isElectric() const = 0;
      virtual bool canSteer() const = 0;
      virtual byte getIndex() const = 0;
      virtual bool isInverse() const = 0;
};
#endif
