#ifndef LOGGER_WRITER_H
#define LOGGER_WRITER_H

#include <Arduino.h>
#include "../interfaces/interfaces.hpp"

class Logger_writer : public virtual ISend
{
public:
  Logger_writer() : ISend()  {
  }

  void write(String msg)
  {
    Logger::Instance().write(LogLevel::INFO, msg);
  }
};

#endif
