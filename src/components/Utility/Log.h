#ifndef Log_h
#define Log_h
// Source: http://www.yolinux.com/TUTORIALS/C++Singleton.html
//Usage Logger::Instance()->method(<param>);
#include "../Communication/WebSocketCommunicator.h"
#include <Metro.h>

class Log
{
public:

 Metro* logInterval;
 static Log* Instance();
 // Public Methods..
 void logTo(const char* target); // TODO:: This should take an enum paramenter. Case we choose WEB_SOCKET_CLIENTS then we enable websockets.
 void enableWebsocket(WebSocketCommunicator* wsCommunicator);
 void enable();
 void disable();
 void write(String payload);
 void logAccel(float average, byte newSpeed, byte targetSpeed, byte previousSpeed, long motorRpm);
private:
  Log(){}; // Private so that it can not be called
  Log(Log const&){};// copy constructor is private
  Log& operator=(Log const&){}; //assignment operator is private.
  static Log* m_pInstance; // Not sure what this is.
  bool isEnabled = false;
  WebSocketCommunicator* wsCommunicator;
};


#endif
