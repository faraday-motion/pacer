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
  }

  return log;
}


void Log::setTarget(Log::Target target)
{
  this->logTarget = target;
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

  // Log to Serial.
  if ((this->logTarget & Log::Target::SERIAL_LOG)  == Log::Target::SERIAL_LOG) {
    Serial.println(toLog);
  }

  // Log to Websocksets
  if ((this->logTarget & Log::Target::WEBSOCKETS) == Log::Target::WEBSOCKETS) {
    // TODO:: Log to websockets
    Serial.println("This should be logged to websockets");
    Serial.println(toLog);
  }

  // Log to file
  if ((this->logTarget & Log::Target::LOG_FILE) == Log::Target::LOG_FILE) {
    // TOOD:: Log to file
    Serial.println("This should be logged to file");
    Serial.println(toLog);
  }

}


// void Log::logAccel(float average, byte newSpeed, byte targetSpeed, byte previousSpeed, long motorRpm)
// {
//     if (isEnabled)
//     {
//       String a = (String)average;
//
//       String message =
//         "AVRG: " + (String)average + " " +
//         " newSpd: " + (String)newSpeed +
//         " lockTr: " +  (String)targetSpeed +
//         " currSpd: " + (String)previousSpeed +
//         " RPM: " + (String)motorRpm;
//       wsCommunicator->wss->broadcastTXT(message);
//     }
//
// }


void Log::enableWebsocket(WebSocketCommunicator* wsCommunicator)
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
