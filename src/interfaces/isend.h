#ifndef ISEND_H
#define ISEND_H

#include <Arduino.h>

class ISend {
   public:
      virtual void send(String message) = 0;
};
#endif
