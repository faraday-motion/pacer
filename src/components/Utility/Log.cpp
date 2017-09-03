 #include "Log.h"
#include <Arduino.h> // for Serial
// Global static pointer used to ensure a single instance of the class.
Log* Log::log = nullptr;

/**
    This function is called to create an instance of the class.
    Calling the constructor publicly is not allowed. The constructor
    is private and is only called by this Instance function.
*/

Log* Log::Logger()
{
  if(!log){ // Only allow one instance of class to be generated.
    log = new Log;
    log->addTarget(Log::Target::SERIAL_LOG);
  }

  return log;
}


void Log::addTarget(Log::Target target)
{
  for (byte i = 0; i < 3; i++)
  {
    if (this->logTargets[i] == target)
      break; // target already added.

    if (this->logTargets[i] == Log::Target::NULL_TARGET) {
      this->logTargets[i] = target;
      return;
    }
  }

  this->write(Log::Level::DEBUG, "No more log target slots");
}

void Log::removeTarget(Log::Target target)
{
  for (byte i = 0; i < 3; i++)
  {
    if (this->logTargets[i] == target) {
      this->logTargets[i] = Log::Target::NULL_TARGET;
    }
  }

}

void Log::setLevel(Log::Level level)
{
  this->logLevel = level;
}

bool Log::setFile(String fileName)
{
  //TODO:: Implement check with the SPIFFS.
  return true;
}

Log::Level Log::getLevel()
{
  return this->logLevel;
}

String Log::levelToString(Log::Level level)
{
  String stringLevel = "Unknown Level";

  if (level == 1)
    stringLevel = "DEBUG";
  if (level == 2)
    stringLevel = "INFO";
  if (level == 3)
    stringLevel = "NOTICE";
  if (level == 4)
    stringLevel = "WARNING";
  if (level == 5)
    stringLevel = "ERR";

  return stringLevel;
}

void Log::write(Log::Level level, String message)
{
  String toLog;
  // Only log if the logger is enabled.
  if (!this->isEnabled)
    return;

  // Only log if we're at or above teh pre-defined severtity
  if (level < this->logLevel)
    return;

  // Append the log level
  toLog += this->levelToString(level);

  // Append the message to our log statement
  toLog += ":: " + message;

  for (byte i = 0; i < 3; i++) {
    // Log to Serial.
    if ((this->logTargets[i] & Log::Target::SERIAL_LOG)  == Log::Target::SERIAL_LOG) {
      Serial.println(toLog);
    }

    // Log to Websocksets
    if ((this->logTargets[i] & Log::Target::WEBSOCKETS) == Log::Target::WEBSOCKETS) {
      if (this->wsCommunicator == nullptr) {
        return;
      }

      // TODO:: The wsCommunicator should keep track of clients. We should not broadcast
      this->wsCommunicator->wss->broadcastTXT(toLog);
    }

    // Log to file
    if ((this->logTargets[i] & Log::Target::LOG_FILE) == Log::Target::LOG_FILE) {
      // TOOD:: Log to file
      Serial.println("This should be logged to file");
      Serial.println(toLog);
    }
  }



}


void Log::bindWebscoket(WebSocketCommunicator* wsCommunicator)
{
  this->wsCommunicator = wsCommunicator;
}

void Log::enable()
{
  this->isEnabled = true;
}

void Log::disable()
{
  this->isEnabled = false;
}
