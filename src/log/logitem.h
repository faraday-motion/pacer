#ifndef LOGITEM_H
#define LOGITEM_H

#include <vector>
#include <Arduino.h>
#include "../interfaces/interfaces.hpp"
#include "../enums/enums.hpp"

class LogItem
{
public:

  LogItem(LogLevel level, String message)
  {
    this -> level = level;
    this -> message = message;
  }
  LogLevel level;
  String message;
};

#endif
