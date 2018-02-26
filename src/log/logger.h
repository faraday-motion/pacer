#ifndef LOGGER_H
#define LOGGER_H

#include <vector>
#include <queue>
#include <Arduino.h>
#include "../interfaces/interfaces.hpp"
#include "../enums/enums.hpp"
#include "./logitem.h"

class Logger
{
public:

  static Logger &Instance()
  {
      // This line only runs once, thus creating the only instance in existence
      static Logger *instance = new Logger;
      // dereferencing the variable here, saves the caller from having to use
      // the arrow operator, and removes temptation to try and delete the
      // returned instance.
      return *instance; // always returns the same instance
  }

 // Add the location to log messages.
 // @param Target, the log Target.
 void addLog(ILog* target)
 {
   if (target != nullptr)
     loggerVector.push_back(target);
 }

 void clearLoggers()
 {
    loggerVector.clear();
 }

 // Convert the Log::Level enum to a string.
 // @parapm Log::Level, the level to convert
 // @return string , the converted level
 String levelToString(LogLevel level);

 // Log a message
 // @param Level, the severity of the message
 // @param String, the message.
 void write(LogLevel level, String message, String value = "");

 void setBufferSize(int size)
 {
    if (size < 0)
     size = 0;
    mLogBufferSize = size;
 }
protected:
  Logger(){}; // Constructor is protected
  Logger(Logger const&){};// copy constructor is protected
  Logger& operator=(Logger const&){}; //assignment operator is protected.
private:
  std::vector<ILog*> loggerVector;
  std::queue<LogItem*> logItemVector;
  int mLogBufferSize = LOGGER_LOG_BUFFER;
};

#endif
