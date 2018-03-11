#ifndef ISENSORVALUE_H
#define ISENSORVALUE_H

#include <Arduino.h>

class ISensorValue {
   public:
      virtual String getName() = 0;
      virtual String getStringValue() = 0;
};
#endif
