#ifndef ILOG_H
#define ILOG_H

#include <Arduino.h>
#include "../enums/loglevel.h"
#include "./ienableable.h"

class ILog {
   public:
      virtual void write(LogLevel level, String message) = 0;
};
#endif
