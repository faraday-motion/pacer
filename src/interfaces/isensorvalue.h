#ifndef ISENSORVALUE_H
#define ISENSORVALUE_H

#include <Arduino.h>

class ISensorvalue {
   public:
      virtual String getName() = 0;
      virtual String getStringValue() = 0;
};
#endif
