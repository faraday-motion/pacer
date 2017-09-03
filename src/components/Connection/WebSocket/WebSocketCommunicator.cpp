#include "components/Utility/Log.h"
#include "WebSocketCommunicator.h"
#include "components/Console/Console.h"

using namespace std::placeholders;

WebSocketCommunicator::WebSocketCommunicator(ConfigController* configController)
{
  Log::Logger()->write(Log::Level::INFO, "Setting up webSocketServer");
  this->configController = configController;
  this->wss = new WebSocketsServer(configController->config->websocket.port);
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
        //Serial.print("New Client Connectd with IP = ");
        // Serial.println(wss->remoteIP(num));
        // Serial.println("Listing Clintes:");
        // for (uint8_t i=0; i < WEBSOCKETS_SERVER_CLIENT_MAX  ; i++) {
        //   if (wss->remoteIP(i) )
        //       Serial.println(wss->remoteIP(i));
        // }
        wss->sendTXT(num, "You are connected");
      }
      break;

    case WStype_TEXT:
      {
        Log::Logger()->write(Log::Level::DEBUG, "Received Websocket Message");

        //Serial.printf("[%u] get Text: %s\n", num, payload);
        String message = (char *)payload;
        byte splitIndex = message.indexOf(':');
        String command = message.substring(0, splitIndex);
        String data = message.substring(splitIndex + 1);

        // Handle Received command
        String response = Console::Cmd()->handle(command.toInt(), data);
        Log::Logger()->write(Log::Level::DEBUG, "repsonse after Console:handleCommand() :: ");
        Log::Logger()->write(Log::Level::DEBUG, response);

        yield();
        this->wss->sendTXT(this->clientId, response);
        yield();
      }
      break;

    case WStype_BIN:
      //hexdump(payload, length);
      //wss->sendBIN(num, payload, length);
      break;
  }
}
