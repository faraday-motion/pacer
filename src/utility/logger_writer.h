#ifndef LOGGER_WRITER_H
#define LOGGER_WRITER_H

#include <Arduino.h>
#include "../interfaces/interfaces.hpp"

class Logger_writer : virtual public IWrite
{
public:
  Logger_writer() : IWrite()  {
  }

  void write(String msg)
  {
    Logger::Instance().write(LogLevel::INFO, msg);
  }
};

#endif
