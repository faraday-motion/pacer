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
}


void WebSocketCommunicator::onWsEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{
  switch (type) {
    case WStype_DISCONNECTED:
      break;
    case WStype_CONNECTED:
      {
        this->clientId = num; // TODO:: We need to keep track of websocket clients.
        Log::Logger()->write(Log::Level::DEBUG, "New Websocket Client Connectd with IP = " + this->ipAddressToString(wss->remoteIP(num)));
        Log::Logger()->write(Log::Level::DEBUG, "Assigned Client ID = " + (String)this->clientId);
        
        Log::Logger()->write(Log::Level::DEBUG, "Connected Clients: ");
        for (uint8_t i = 0; i < WEBSOCKETS_SERVER_CLIENT_MAX  ; i++) {
          IPAddress clientIp = wss->remoteIP(i);          
          Log::Logger()->write(Log::Level::DEBUG,"Client ID: " + (String)i);
          Log::Logger()->write(Log::Level::DEBUG,"Client IP: " + this->ipAddressToString(clientIp));
        }
      }
      break;

    case WStype_TEXT:
      {
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

        this->wss->sendTXT(this->clientId, response);

      }
      break;

    case WStype_BIN:
      //hexdump(payload, length);
      //wss->sendBIN(num, payload, length);
      break;
  }
}


String WebSocketCommunicator::ipAddressToString(IPAddress ip)
{
  return (String)ip[0] + "." +
         (String)ip[1] + "." +
         (String)ip[2] + "." +
         (String)ip[3] + "." +
         (String)ip[4];
}
