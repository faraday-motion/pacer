#ifndef IWRITE_H
#define IWRITE_H

#include <Arduino.h>

class IWrite {
   public:
      virtual void write(String msg) = 0;
};
#endif
