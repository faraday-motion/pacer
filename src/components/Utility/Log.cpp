#include "Log.h"
#include <Arduino.h> // for Serial
// Global static pointer used to ensure a single instance of the class.
Log* Log::m_pInstance = nullptr;


/** This function is called to create an instance of the class.
    Calling the constructor publicly is not allowed. The constructor
    is private and is only called by this Instance function.
*/

Log* Log::Instance()
{
  if(!m_pInstance) // Only allow one instance of class to be generated.
    m_pInstance = new Log;
  return m_pInstance;
}

/**
  Public Methods..
*/

void Log::write(String payload)
{
  if(isEnabled)
  {
    Serial.println(payload);
    // TODO:: Check if wsCommunicator extists.
    wsCommunicator->wss->broadcastTXT("payload of logged data");
  }
}
// void Log::write(String payload)
// {
//   if(isEnabled){
//     wsCommunicator->wss->broadcastTXT(payload);
//   }
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
