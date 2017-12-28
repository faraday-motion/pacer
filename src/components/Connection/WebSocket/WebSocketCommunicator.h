#ifndef WebSocket_h
#define WebSocket_h

#include <Arduino.h>
#include <Hash.h> // Important! Hash.h needs to be included after <Arduino.h>
#include <ESP8266WiFi.h>
#include "ArduinoJson.h"
#include <WebSocketsServer.h>
#include "components/Console/Commands.h"
#include "components/Config/Config.h"

// CONNECTION_EVICT_TIMEOUT must be greater than CONNECTION_PING_TIMEOUT
#define CONNECTION_PING_TIMEOUT (30000)
#define CONNECTION_EVICT_TIMEOUT (60000)
#define PING_PAYLOAD ("PING")
typedef struct {
  uint32_t lastContact;
  bool pinged;
  bool connected;
} WSClientPingStatus;
// A bit of a Hack due to https://github.com/Links2004/arduinoWebSockets/issues/14
class WebSocketCommunicator {
private:
  WSClientPingStatus clientStatues[WEBSOCKETS_SERVER_CLIENT_MAX];
  String ipAddressToString(IPAddress ip);
  bool handleMaxConnections(int num);
  void clientContacted(int num);
  void clientConnected(int num);
  void clientDisconnected(int num);
  void evictClient(int num);
  void pingClient(int num);
public:
  byte subscribers[];
  WebSocketsServer* wss;
  WebSocketCommunicator();
  void onWsEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);
  void EvictInactiveConnections();
};

#endif
