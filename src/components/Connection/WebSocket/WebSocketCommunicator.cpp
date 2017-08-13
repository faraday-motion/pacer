#include "components/Utility/Log.h"
#include "WebSocketCommunicator.h"


using namespace std::placeholders;

WebSocketCommunicator::WebSocketCommunicator(ConfigController* configController)
{
  this->wss = new WebSocketsServer(configController->config->websocket.port);
  this->wss->onEvent(std::bind(&WebSocketCommunicator::onWsEvent, this, _1, _2, _3, _4));
}


void WebSocketCommunicator::onWsEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{
  switch (type) {
    case WStype_DISCONNECTED:
      break;
    case WStype_CONNECTED:
      {
        Serial.print("New Client Connectd with IP = ");
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
        Serial.println("Received a Websocket Command");
        Serial.println((char *) &payload[0]);
        // Text Data has been received.
        String command = (const char *) &payload[0]; // TODO:: Find a better way of casting the payload data type.
        wss->sendTXT(num, handleCommand(command.toInt()));
        yield();
      }
      break;

    case WStype_BIN:
      //hexdump(payload, length);
      //wss->sendBIN(num, payload, length);
      break;
  }
}



const char* WebSocketCommunicator::handleCommand(unsigned int command)
{
  const char* response;
  Serial.print("Processing Command:: ");
  Serial.println(command);
  switch (command) {
    case 8080:
      // Enable the logger
      Serial.println("Command:: Enable Logger");
      Log::Instance()->enable();
      response = "Logger Enabled";
      break;
    case 8181:
      Serial.println("Command:: Disable Logger");
      // Disable the logger here.
      Log::Instance()->disable();
      response = "Logger Disabled";
      break;
    default: response = "Unknown Command";
  }

  return response;
}
