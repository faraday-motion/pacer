#ifndef Log_h
#define Log_h

#include "components/Connection/WebSocket/WebSocketCommunicator.h"

class Log
{
public:

  // Available Targets.
 enum Target : byte {
    NULL_TARGET = 0,
    SERIAL_LOG = 1, // can't use SERIAL.
    WEBSOCKETS = 2,
    LOG_FILE = 3
  };

  // Log Levels.
 enum Level : byte {
    DEBUG = 1,
    INFO = 2,
    NOTICE = 3,
    WARNING = 4,
    ERR = 5
  };

 // Get signleton instance
 static Log* Logger();

 // Add the location to log messages.
 // @param Target, the log Target.
 void addTarget(Target target);

 // Remove the location to log messages.
 // @param Target, the log Target.
 void removeTarget(Target target);

 // Set the severity of the message to log.
 // @param Level, the log level to set
 void setLevel(Level level);

 // Set a file to log to if the the target is LOG_FILE
 // @param string, The file path.
 bool setFile(String fileName);

 // Get the current log level. Only messages
 // with equal or higher severity will be written.
 // @return string, the converted level.
 Level getLevel();

 // Convert the Log::Level enum to a string.
 // @parapm Log::Level, the level to convert
 // @return string , the converted level
 String levelToString(Level level);

 // Log a message
 // @param Level, the severity of the message
 // @param String, the message.
 void write(Level level, String message);

 // Bind the WebsocketServer Object to the logger as a target.
 // @param WebSocketCommunicator, the WebSocketCommunicator Object
 void bindWebscoket(WebSocketCommunicator* wsCommunicator);
 void enable();
 void disable();
 void write(String payload);

protected:
  Log(){}; // Private so that it can not be called
  Log(Log const&){};// copy constructor is protected
  Log& operator=(Log const&){}; //assignment operator is protected.
private:
  static Log* log; // The log instance.

  // write() uses these variables to determine which messages should be written where.
  Level logLevel = Level::WARNING; // default log level.
  Target logTargets[3]; // default log target.
  String logFile = "";

  // TODO:: Manually convert levels to strings.

  bool isEnabled = false;
  WebSocketCommunicator* wsCommunicator = nullptr;
};


#endif
