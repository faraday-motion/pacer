#ifndef WebSocket_h
#define WebSocket_h

#include <Arduino.h>
#include <Hash.h> // Important! Hash.h needs to be included after <Arduino.h>
#include <ESP8266WiFi.h>
#include "ArduinoJson.h"
#include <WebSocketsServer.h>
#include "components/Console/Commands.h"
#include "components/Config/Config.h"


// A bit of a Hack due to https://github.com/Links2004/arduinoWebSockets/issues/14
class WebSocketCommunicator {
private:
  uint8_t clientId;
public:
  byte subscribers[];
  WebSocketsServer* wss;

  WebSocketCommunicator();
  void onWsEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);
};

#endif
