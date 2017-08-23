#include "components/Utility/Log.h"
#include "WebSocketCommunicator.h"

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
      Log::Logger()->write(Log::Level::DEBUG, "Command:: Enable Logger");
      Log::Logger()->enable();
      response = "Logger Enabled";
      break;
    case DISABLE_LOGGER:
      // Disable the logger here.
      Log::Logger()->write(Log::Level::DEBUG, "Command:: Disable Logger");
      Log::Logger()->disable();
      response = "Logger Disabled";
      break;
    case ADD_LOGGER_TARGET:
      if(data == "SERIAL_LOG")
        Log::Logger()->addTarget(Log::Target::SERIAL_LOG);
      if(data == "WEBSOCKETS")
        Log::Logger()->addTarget(Log::Target::WEBSOCKETS);
      if(data == "LOG_FILE")
        Log::Logger()->addTarget(Log::Target::LOG_FILE);
      break;
    case REMOVE_LOGGER_TARGET:
      if(data == "SERIAL_LOG")
        Log::Logger()->removeTarget(Log::Target::SERIAL_LOG);
      if(data == "WEBSOCKETS")
        Log::Logger()->removeTarget(Log::Target::WEBSOCKETS);
      if(data == "LOG_FILE")
        Log::Logger()->removeTarget(Log::Target::LOG_FILE);
      break;
    case SET_LOGGER_LEVEL:
      if(data == "DEBUG")
        Log::Logger()->setLevel(Log::Level::DEBUG);
      if(data == "INFO")
        Log::Logger()->setLevel(Log::Level::INFO);
      if(data == "NOTICE")
        Log::Logger()->setLevel(Log::Level::NOTICE);
      if(data == "WARNING")
        Log::Logger()->setLevel(Log::Level::WARNING);
      if(data == "ERR")
        Log::Logger()->setLevel(Log::Level::ERR);
      break;
    case GET_CONFIG:
      Log::Logger()->write(Log::Level::DEBUG, "Command:: Get Config");
      config = this->configController->getRawConfig();
      wss->sendTXT(this->clientId, config);
      break;
    case SET_CONFIG:
      Log::Logger()->write(Log::Level::DEBUG, "Command:: Set Config");
      this->configController->writeRawConfig(data);
      wss->sendTXT(this->clientId, "Trying to rewrite the configuration.");
      break;
    default: response = "Unknown Command";
  }

  return response;
}
