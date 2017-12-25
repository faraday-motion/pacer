#include "Arduino.h"
#include "WebSocketCommunicator.h"
#include "components/Utility/Log.h"
#include "components/Console/Console.h"

using namespace std::placeholders;

WebSocketCommunicator::WebSocketCommunicator()
{
  Config* config = Config::get();
  Log::Logger()->write(Log::Level::INFO, "Setting up webSocketServer");
  this->wss = new WebSocketsServer(config->websocket.port);
  this->wss->onEvent(std::bind(&WebSocketCommunicator::onWsEvent, this, _1, _2, _3, _4));
  Log::Logger()->write(Log::Level::INFO, "Finished setting up webSocketServer"); 
  for (uint8_t i = 0; i <  WEBSOCKETS_SERVER_CLIENT_MAX  ; i++) 
    this->clientStatues[i].connected = false;
}

void WebSocketCommunicator::onWsEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{
  switch (type) {
    case WStype_DISCONNECTED:
      this->clientDisconnected(num);
      break;
    case WStype_CONNECTED:
      {
        if (!handleMaxConnections(num)) break;
        this->clientConnected(num);  

        Log::Logger()->write(Log::Level::DEBUG, "New Websocket Client Connectd with IP = " + this->ipAddressToString(wss->remoteIP(num)));
        Log::Logger()->write(Log::Level::DEBUG, "Assigned Client ID = " + (String)num);
        Log::Logger()->write(Log::Level::DEBUG, "Connected Clients: ");
        for (uint8_t i = 0; i <  WEBSOCKETS_SERVER_CLIENT_MAX  ; i++) {
          IPAddress clientIp = wss->remoteIP(i);          
          Log::Logger()->write(Log::Level::DEBUG,"Client ID: " + (String)i);
          Log::Logger()->write(Log::Level::DEBUG,"Client IP: " + this->ipAddressToString(clientIp));
        }
      }
      break;
    case WStype_TEXT:
      {
        this->clientContacted(num);
        //Serial.printf("[%u] get Text: %s\n", num, payload);
        String message = (char *)payload;
        StaticJsonBuffer<1200> jsonBuffer;
        JsonObject& json = jsonBuffer.parse((char *)payload);
        if (!json.success()){
          Log::Logger()->write(Log::Level::ERR, "Failed to parse json websocket message.");
          break; // Don't pass the json further into the app.
        }
        unsigned int id = json["id"];
        unsigned int command = json["command"];
        String data = json["payload"];

        // Handle Received commands
        Log::Logger()->write(Log::Level::DEBUG,"Websocket Packet Received");
        Log::Logger()->write(Log::Level::DEBUG, "id:" + (String)id );
        Log::Logger()->write(Log::Level::DEBUG, "command: " + (String)id );
        Log::Logger()->write(Log::Level::DEBUG, "payload: " + data);
        
        String response = Console::Cmd()->handle(id, command, data);
        Log::Logger()->write(Log::Level::DEBUG, "Websocket resonse: " + response);

        this->wss->sendTXT(num, response);
      }
      break;

    case WStype_BIN:
      //hexdump(payload, length);
      //wss->sendBIN(num, payload, length);
      break;
  }
}

void WebSocketCommunicator::EvictInactiveConnections()
{
  uint32_t now = millis();
  uint32_t deltaTime;
  for (uint8_t i = 0; i < WEBSOCKETS_SERVER_CLIENT_MAX  ; i++) 
  {
    if (!this->clientStatues[i].connected) continue;
    deltaTime = now - this->clientStatues[i].lastContact;
    if (deltaTime > CONNECTION_EVICT_TIMEOUT)
    {
      this->evictClient(i);
    }
    else if (deltaTime > CONNECTION_PING_TIMEOUT && !this->clientStatues[i].pinged)
    {
      this->pingClient(i);
    }
  }
}

void WebSocketCommunicator::pingClient(int num)
{
  this->wss->sendTXT(num, PING_PAYLOAD);
  this->clientStatues[num].pinged=true;
}

void WebSocketCommunicator::evictClient(int num)
{
  this->wss->disconnect(num);
  this->clientDisconnected(num);
}

void WebSocketCommunicator::clientConnected(int num)
{
  this->clientStatues[num].connected=true;
  this->clientStatues[num].pinged=false;
  this->clientStatues[num].lastContact = millis();
}

void WebSocketCommunicator::clientDisconnected(int num)
{
  this->clientStatues[num].connected=false;
  this->clientStatues[num].pinged=false;
}

void WebSocketCommunicator::clientContacted(int num)
{
  this->clientStatues[num].lastContact=millis();
}

bool WebSocketCommunicator::handleMaxConnections(int num)
{
  if (num==WEBSOCKETS_SERVER_CLIENT_MAX-1)
  {
    evictClient(num);
    return false;
  }
  return true;  
}

String WebSocketCommunicator::ipAddressToString(IPAddress ip)
{
  return (String)ip[0] + "." +
         (String)ip[1] + "." +
         (String)ip[2] + "." +
         (String)ip[3] + "." +
         (String)ip[4];
}
