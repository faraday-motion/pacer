#ifndef WEBSOCKET_WRITER_H
#define WEBSOCKET_WRITER_H

#include <Arduino.h>
#include <WebSocketsServer.h>
#include "../../interfaces/interfaces.hpp"

class Websocket_writer : virtual public IWrite
{
private:
  WebSocketsServer* mWebSocketsServer = nullptr;
  uint8_t mNum;
protected:

public:
  Websocket_writer(WebSocketsServer* webSocketsServer = nullptr) : IWrite()  {
    mWebSocketsServer = webSocketsServer;
  }

  void setClientId(uint8_t num)
  {
    mNum = num;
  }

  void write(String msg)
  {
    if (mWebSocketsServer != nullptr)
      mWebSocketsServer -> sendTXT(mNum, msg);
  }
};

#endif
