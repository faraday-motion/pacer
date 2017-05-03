#ifndef WebSocket_h
#define WebSocket_h
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <Hash.h>


// A bit of a Hack due to https://github.com/Links2004/arduinoWebSockets/issues/14
class WebSocketCommunicator {
public:
  byte subscribers[];
  
  WebSocketsServer* wss;
  WebSocketCommunicator(byte port);
  void onWsEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);
  const char*  handleCommand(unsigned int command);
};

#endif
