#ifndef LOG_MODULE_H
#define LOG_MODULE_H
#include <Arduino.h>
#include "./base.hpp"

class Log_module : public Modulebase, public ILog
{
private:
  LogLevel mLogLevel = LogLevel::DEBUG; // default log level.
protected:
  //ISend * pSender = nullptr;
  Log_module(byte id, int module) : Modulebase(id, module, Roles::LOG_MODULE) {

  };
public:
    // write() uses these variables to determine which messages should be written where.
    // Set the severity of the message to log.
    // @param Level, the log level to set
    void setLevel(LogLevel level)
    {
      mLogLevel = level;
    }

    // Get the current log level. Only messages
    // with equal or higher severity will be written.
    // @return string, the converted level.
    LogLevel getLevel()
    {
      return mLogLevel;
    }

    //void setSender(ISend * sender);
    virtual void write(LogLevel level, String message)
    {
      
    }
};

#endif
