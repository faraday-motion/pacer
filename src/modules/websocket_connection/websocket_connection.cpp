
#include <Arduino.h>
#include <ArduinoJson.h>
#include "./websocket_connection.h"
#include "../../configuration/default/configuration.h"
#include "../../configuration/configurator.h"
#include "../../enums/externalcommands.h"
#include "../../utility/tools.h"
#include "../../utility/spiffs_storage.h"

using namespace std;
using namespace std::placeholders;

void Websocket_connection::setup() {
  if (mIsSetup == false)
  {
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up Websocket_connection"));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
//    onEvent(Events::CONFIGURE, true);
    mWebSocketsServer = new WebSocketsServer(mPort);
    Logger::Instance().write(LogLevel::INFO, FPSTR("Websocket on port: "), String(mPort));
    mWebSocketsServer -> begin();
    mWebSocketsServer -> onEvent(std::bind(&Websocket_connection::onWsEvent, this, _1, _2, _3, _4));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up Websocket_connection"));
    mIsSetup = true;
  }
}

void Websocket_connection::loop()
{
  if (enabled())
  {
    if (mSimpleTimer.check())
    {
      Logger::Instance().write(LogLevel::DEBUG, FPSTR("Websocket_connection::loop"));
      mWebSocketsServer -> loop();
    }
  }
}

void Websocket_connection::send(String message)
{
  if (enabled())
  {
    Logger::Instance().write(LogLevel::DEBUG, FPSTR("Websocket_connection::send: "), message);
    message = "{\"response\":" + message + "}";
    for (uint8_t i = 0; i < pClients.size(); i++) {
      if (mWebSocketsServer != nullptr)
        mWebSocketsServer -> sendTXT(pClients[i], message);
    }
  }
}

void Websocket_connection::onWsEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{
  //num is clientid
  switch (type) {
    case WStype_DISCONNECTED:
        removeClient(num);
      break;
    case WStype_CONNECTED:
      {
        Logger::Instance().write(LogLevel::DEBUG, FPSTR("New Websocket Client Connectd with IP = "), Tools::ipAddressToString(mWebSocketsServer -> remoteIP(num)));
        Logger::Instance().write(LogLevel::DEBUG, FPSTR("Assigned Client ID = "), String(num));
        addClient(num);

        Logger::Instance().write(LogLevel::DEBUG, FPSTR("Connected Clients: "));
        for (uint8_t i = 0; i < pClients.size(); i++) {
          IPAddress clientIp = mWebSocketsServer -> remoteIP(i);
          Logger::Instance().write(LogLevel::DEBUG, FPSTR("Client ID: "), String(i));
          Logger::Instance().write(LogLevel::DEBUG, FPSTR("Client IP: "), Tools::ipAddressToString(clientIp));
        }
        String response = FPSTR("{\"CONNECTED\":\"true\"}");
        Logger::Instance().write(LogLevel::DEBUG, FPSTR("Websocket resonse: "), response);
        mWebSocketsServer -> sendTXT(num, response);
      }
      break;
    case WStype_TEXT:
      {
        String message = (char *)payload;
        StaticJsonBuffer<1200> jsonBuffer;
        JsonObject& json = jsonBuffer.parse((char *)payload);

        if (!json.success()){
          Logger::Instance().write(LogLevel::ERROR, FPSTR("Failed to parse json websocket message."));
          //TODO Return error message
          break;
        }
        byte command = json["command"];
        byte value = json["value"];

        Logger::Instance().write(LogLevel::DEBUG, FPSTR("Websocket Packet Received"));
        Logger::Instance().write(LogLevel::DEBUG, FPSTR("command: "), String(command) + " value: " + String(value));

        //For each reciever do
        for (byte i=0; i < recievers().size(); i++) {
          recievers()[i] -> recieve(command, value);
        }

        if (command == 100)
        {
          Spiffs_storage storage;
          //storage.read("/drivelog.txt", pWriter);
        }

        String response = "{\"command\":\"" + String(command) + "\",\"response\": \"" + "OK" + "\"}";
        mWebSocketsServer -> sendTXT(num, response);
        Logger::Instance().write(LogLevel::DEBUG, FPSTR("Websocket resonse: "), response);
      }
      break;
    case WStype_BIN:
      break;
  }
}

void Websocket_connection::command(byte command)
{

}

void Websocket_connection::onDisable()
{

}
