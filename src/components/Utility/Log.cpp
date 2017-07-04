#include "Log.h"
#include <Arduino.h> // for Serial
// Global static pointer used to ensure a single instance of the class.
Log* Log::m_pInstance = nullptr;


/**
    This function is called to create an instance of the class.
    Calling the constructor publicly is not allowed. The constructor
    is private and is only called by this Instance function.
*/

Log* Log::Instance()
{
  if(!m_pInstance){ // Only allow one instance of class to be generated.
    m_pInstance = new Log;
    m_pInstance->logInterval = new Metro(1000); // This should be configurable.
  }

  return m_pInstance;
}

/**
  Public Methods..
*/

void Log::write(String payload)
{
  if(isEnabled)
  {
    //wsCommunicator->wss->broadcastTXT(payload);
  }
}
// void Log::write(String payload)
// {
//   if(isEnabled){
//     wsCommunicator->wss->broadcastTXT(payload);
//   }
// }

void Log::logAccel(float average, byte newSpeed, byte targetSpeed)
{
    //if (isEnabled)
    //{
      String a = (String)average;

      String message = "average " + (String)average + " " + " newSpeed " + (String)newSpeed +  " lockedTarget " +  (String)targetSpeed;
      wsCommunicator->wss->broadcastTXT(message);
    //}

}


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
