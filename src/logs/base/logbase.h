#ifndef LOGBASE_H
#define LOGBASE_H

#include <Arduino.h>
#include "../../configuration/default/configuration.h"
#include "../../enums/enums.hpp"
#include "../../interfaces/interfaces.hpp"

class Logbase : public ILog
{
private:
  bool mIsEnabled = LOGGER_LOG_DEFALT_ENABLED;
  LogLevel mLogLevel = LogLevel::DEBUG; // default log level.
protected:
  byte mId = 0;
  ISend * pSender = nullptr;
  Logbase(byte id) : ILog() {
    mId = id;
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

    void setSender(ISend * sender);
    virtual void write(LogLevel level, String message);

    void setEnabled(bool enabled)
    {
      mIsEnabled = enabled;
    }

    bool enabled()
    {
      return mIsEnabled;
    }
};

#endif
