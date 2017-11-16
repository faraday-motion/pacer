#ifndef WsController_h
#define WsController_h
#include "../AbstractController.h"
#include "components/Connection/WebSocket/WebSocketCommunicator.h"

class WsController : public AbstractController
{
private:
  // Intervals
  unsigned int _LOST_CONNECTION = 500; // TODO:: This should only be on the base class.

  // Physical Device Identification
  AbstractDevice client;

  Metro* connectionLostTimer; 

  byte input = 50;

public: 
  WebSocketCommunicator* _ws;
  WsController(WebSocketCommunicator* ws, AbstractDevice device);

  void setInput(byte input);

  bool handleController();
  bool enable();
  bool disable();
};

#endif