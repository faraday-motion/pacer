#include "components/Utility/Log.h"
#include "WebSocketCommunicator.h"

using namespace std::placeholders;

WebSocketCommunicator::WebSocketCommunicator(ConfigController* configController)
{
  Serial.println("Setting up webSocketServer");
  this->configController = configController;
  this->wss = new WebSocketsServer(configController->config->websocket.port);
  this->wss->onEvent(std::bind(&WebSocketCommunicator::onWsEvent, this, _1, _2, _3, _4));
  Serial.println("Finished setting up webSocketServer");
}


void WebSocketCommunicator::onWsEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{
  switch (type) {
    case WStype_DISCONNECTED:
      break;
    case WStype_CONNECTED:
      {
        this->clientId = num; // TODO:: We need to keep track of websocket clients.
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
        Serial.println("Received a Websocket Message");
        //Serial.printf("[%u] get Text: %s\n", num, payload);
        String message = (char *)payload;
        byte splitIndex = message.indexOf(':');
        String command = message.substring(0, splitIndex);
        String data = message.substring(splitIndex + 1);
        // Handle Received command
        this->handleCommand(command.toInt(), data);
        yield();
      }
      break;

    case WStype_BIN:
      //hexdump(payload, length);
      //wss->sendBIN(num, payload, length);
      break;
  }
}



const char* WebSocketCommunicator::handleCommand(unsigned int command, String data)
{
  const char* response;
  String config; // hold the raw config
  switch (command) {
    case ENABLE_LOGGER:
      // Enable the logger
      Serial.println("Command:: Enable Logger");
      Log::Instance()->enable();
      response = "Logger Enabled";
      break;
    case DISABLE_LOGGER:
      Serial.println("Command:: Disable Logger");
      // Disable the logger here.
      Log::Instance()->disable();
      response = "Logger Disabled";
      break;
    case GET_CONFIG:
      Serial.println("Command:: Get Raw Config");
      config = this->configController->getRawConfig();
      wss->sendTXT(this->clientId, config);
      break;
    case SET_CONFIG:
      Serial.println("Command:: Set Raw Config");
      this->configController->writeRawConfig(data);
      wss->sendTXT(this->clientId, "Trying to rewrite the configuration.");
      break;
    default: response = "Unknown Command";
  }

  return response;
}
