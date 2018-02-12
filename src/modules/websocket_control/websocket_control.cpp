
#include <Arduino.h>
#include <ArduinoJson.h>
#include "./websocket_control.h"
#include "./websocket_writer.h"
#include "../../configuration/default/configuration.h"
#include "../../configuration/configurator.h"
#include "../../enums/externalcommands.h"
#include "../../utility/tools.h"
#include "../../utility/spiffs_storage.h"

using namespace std;
using namespace std::placeholders;

void Websocket_control::setup() {
  if (mIsSetup == false)
  {
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up Websocket_control"));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    onEvent(CONFIGURE, true);
    mWebSocketsServer = new WebSocketsServer(mPort);
    Logger::Instance().write(LogLevel::INFO, FPSTR("Websocket on port: "), String(mPort));
    mWebSocketsServer -> begin();
    mWebSocketsServer -> onEvent(std::bind(&Websocket_control::onWsEvent, this, _1, _2, _3, _4));
    pWriter = new Websocket_writer(mWebSocketsServer);
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up Websocket_control"));
    mIsSetup = true;
  }
}

void Websocket_control::loop()
{
  if (enabled())
  {
    if (mSimpleTimer.check())
    {
      Logger::Instance().write(LogLevel::DEBUG, FPSTR("Websocket_control::loop"));
      mWebSocketsServer -> loop();
      if (isActive())
      {
        mFMV -> sensors().add("Active", id());
        Logger::Instance().write(LogLevel::DEBUG, FPSTR("Websocket_control::Power "), String(mOutputControl.getPower()) + " " + String(mOutputControl.getBrake()));
        Logger::Instance().write(LogLevel::DEBUG, FPSTR("Websocket_control::Turning "), String(mOutputControl.getLeft()) + " " + String(mOutputControl.getRight()));
      }
      clientTimeoutCheck();
    }
  }
}

void Websocket_control::onWsEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{
  //num is clientid
  switch (type) {
    case WStype_DISCONNECTED:
      break;
    case WStype_CONNECTED:
      {
        Logger::Instance().write(LogLevel::DEBUG, FPSTR("New Websocket Client Connectd with IP = "), Tools::ipAddressToString(mWebSocketsServer -> remoteIP(num)));
        Logger::Instance().write(LogLevel::DEBUG, FPSTR("Assigned Client ID = "), String(num));

        Logger::Instance().write(LogLevel::DEBUG, FPSTR("Connected Clients: "));
        for (uint8_t i = 0; i < mClientMax; i++) {
          IPAddress clientIp = mWebSocketsServer -> remoteIP(i);
          Logger::Instance().write(LogLevel::DEBUG, FPSTR("Client ID: "), String(i));
          Logger::Instance().write(LogLevel::DEBUG, FPSTR("Client IP: "), Tools::ipAddressToString(clientIp));
        }
        String response = "{\"command\":\"" + String(0) + "\",\"response\": \"" + "CONNECTED" + "\"}";
        Logger::Instance().write(LogLevel::DEBUG, FPSTR("Websocket resonse: "), response);
        mWebSocketsServer -> sendTXT(num, response);
      }
      break;

    case WStype_TEXT:
      {
        pWriter -> setClientId(num);

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
        //String data = json["payload"];

        Logger::Instance().write(LogLevel::DEBUG, FPSTR("Websocket Packet Received"));
        Logger::Instance().write(LogLevel::DEBUG, FPSTR("command: "), String(command) + " value: " + String(value));
        handleCommand(command, value);

        if (command == 100)
        {
          Spiffs_storage storage;
          storage.read("/drivelog.txt", pWriter);
        }

        String response = "{\"command\":\"" + String(command) + "\",\"response\": \"" + "OK" + "\"}";
        mWebSocketsServer -> sendTXT(num, response);
        Logger::Instance().write(LogLevel::DEBUG, FPSTR("Websocket resonse: "), response);
      }
      break;

    case WStype_BIN:
      //hexdump(payload, length);
      //wss->sendBIN(num, payload, length);
      break;
  }
}

void Websocket_control::handleCommand(byte command, byte value)
{
  handleCoreCommand(command, value);
}

void Websocket_control::command(byte command)
{

}

void Websocket_control::onDisable()
{
   mOutputControl.reset();
   onEvent(Events::CONTROL_NONE);
}
