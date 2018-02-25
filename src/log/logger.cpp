#include "Logger.h"
#include <Arduino.h>
#include "../macros.h"
#include "../configuration/default/configuration.h"
#include "./logitem.h"


String Logger::levelToString(LogLevel level)
{
  String stringLevel = "Unknown Level";
  switch(int(level)) {
    case 0 :
      stringLevel = "DEBUG";
    break;
    case 1 :
      stringLevel = "INFO";
    break;
    case 2 :
      stringLevel = "WARNING";
    break;
    case 3 :
      stringLevel = "ERROR";
    break;
    case 4 :
      stringLevel = "FATAL";
    break;
  }
  return stringLevel;
}

void Logger::write(LogLevel level, String message, String value)
{
  LogItem * li = nullptr;
  String loggerMesssage = String(millis()) + " ";
  loggerMesssage += levelToString(level);
  loggerMesssage += " --> " + message + value;
  while (logItemVector.size() > LOGGER_LOG_MAXBUFFER)
  {
    //If the buffer is full, pop the first item
    li = logItemVector.front();
    //Lets delete the object and free the memory
    logItemVector.pop();
    delete li;
  }
  logItemVector.push(new LogItem(level, loggerMesssage));

  bool popItem = true;
  while (popItem == true)
  {
    popItem = false;
    for (int i=0; i<loggerVector.size(); i++)
    {
      li = logItemVector.front();
      if (logItemVector.size() > 0)
      {
        popItem = true;
        loggerVector[i] -> write(li -> level, li -> message);
      }
    }
    if (popItem)
    {
      //Lets delete the object and free the memory
      logItemVector.pop();
      delete li;
    }
  }
}
