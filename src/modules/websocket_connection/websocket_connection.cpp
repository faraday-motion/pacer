
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

//https://jwt.io/#libraries
//https://devcenter.heroku.com/articles/websocket-security
//http://simplyautomationized.blogspot.de/2015/09/5-ways-to-secure-websocket-rpi.html
//https://en.wikipedia.org/wiki/Challenge-Handshake_Authentication_Protocol
//https://gist.github.com/jfromaniello/8418116
//https://github.com/auth0/node-jsonwebtoken
//https://github.com/yutter/ArduinoJWT

void Websocket_connection::setup() {
  if (mIsSetup == false)
  {
    mIsSetup = true;
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up "), getModuleName());
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    onEvent(Events::CONFIGURE);
    mWebSocketsServer = new WebSocketsServer(mCfg -> port);
    Logger::Instance().write(LogLevel::INFO, FPSTR("Websocket on port: "), String(mCfg -> port));
    mWebSocketsServer -> begin();
    mWebSocketsServer -> onEvent(std::bind(&Websocket_connection::onWsEvent, this, _1, _2, _3, _4));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up "), getModuleName());
  }
}

void Websocket_connection::loop()
{
  if (enabled())
  {
    if (mSimpleTimer.check())
    {
      Logger::Instance().write(LogLevel::DEBUG, getModuleName(), FPSTR("::loop"));
      mWebSocketsServer -> loop();
    }
    if (mPingpongSimpleTimer.check())
      checkClients();
  }
}

void Websocket_connection::send(String message)
{
  sendInternal(message);
}

void Websocket_connection::sendInternal(String message)
{
  if (enabled())
  {
    //Not allowed to add any log writes here as it creates a recursive loop.
    message = "{\"msg\":\"ok\"," + message + "}";
    for (uint8_t i = 0; i < WEBSOCKETS_SERVER_CLIENT_MAX  ; i++)
    {
      if (mClientStatus[i].connected)
      {
        if (mWebSocketsServer != nullptr)
          mWebSocketsServer -> sendTXT(i, message);
      }
    }
  }
}

void Websocket_connection::onWsEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{
  //num is clientid
  switch (type) {
    case WStype_DISCONNECTED:
        clientDisconnected(num);
      break;
    case WStype_CONNECTED:
      {
        if (!handleMaxClients(num))
          break;
        Logger::Instance().write(LogLevel::DEBUG, FPSTR("New Websocket Client Connectd with IP = "), Tools::ipAddressToString(mWebSocketsServer -> remoteIP(num)));
        Logger::Instance().write(LogLevel::DEBUG, FPSTR("Assigned Client ID = "), String(num));
        clientConnected(num);

        String response = FPSTR("{\"msg\":\"ok\"}");
        Logger::Instance().write(LogLevel::DEBUG, FPSTR("Websocket resonse: "), response);
        mWebSocketsServer -> sendTXT(num, response);
      }
      break;
    case WStype_TEXT:
      {
        clientContact(num);
        String message = (char *)payload;
        StaticJsonBuffer<1200> jsonBuffer;
        JsonObject& json = jsonBuffer.parse((char *)payload);

        if (!json.success()){
          Logger::Instance().write(LogLevel::ERROR, FPSTR("Failed to parse json websocket message."));
          mWebSocketsServer -> sendTXT(num, "{\"msg\":\"error\"}");
          return;
        }
        String ping = json["ping"].as<String>();
        if (ping == "pong")
        {
          Logger::Instance().write(LogLevel::DEBUG, FPSTR("Websocket Pong Packet Received"));
          return;
        }
        byte id = json["id"];
        byte command = json["command"];
        byte value = json["value"];

        Logger::Instance().write(LogLevel::DEBUG, FPSTR("Websocket Packet Received"));
        Logger::Instance().write(LogLevel::DEBUG, FPSTR("command: "), String(command) + " value: " + String(value));

        //For each reciever do
        for (byte i=0; i < recievers().size(); i++) {
          Logger::Instance().write(LogLevel::DEBUG, FPSTR("Websocket Packet recievers:"), String(i));
          recievers()[i] -> recieve(command, value);
        }
        String response = "";
        if (id > 0)
          response = "{\"msg\":\"ok\",\"id\":\"" + String(id) + "\",\"command\":\"" + String(command) + "\"}";
        else
          response = "{\"msg\":\"ok\",\"command\":\"" + String(command) + "\"}";
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

void Websocket_connection::checkClients()
{
  uint32_t now = millis();
  uint32_t deltaTime;
  for (uint8_t i = 0; i < WEBSOCKETS_SERVER_CLIENT_MAX  ; i++)
  {
    if (mClientStatus[i].connected)
    {
      deltaTime = now - mClientStatus[i].lastContact;
      if (deltaTime > WEBSOCKET_PING_TIMEOUT && !mClientStatus[i].pinged)
        pingClient(i);
      else if (deltaTime > WEBSOCKET_EVICT_TIMEOUT)
        evictClient(i);
    }
  }
}

void Websocket_connection::pingClient(int num)
{
  mWebSocketsServer -> sendTXT(num, "{\"msg\":\"" + String(WEBSOCKET_PING_PAYLOAD) + "\"}");
  mClientStatus[num].pinged = true;
}

void Websocket_connection::evictClient(int num)
{
  mWebSocketsServer -> disconnect(num);
  clientDisconnected(num);
}

void Websocket_connection::clientConnected(int num)
{
  mClientStatus[num].connected = true;
  mClientStatus[num].pinged = false;
  mClientStatus[num].lastContact = millis();
}

void Websocket_connection::clientDisconnected(int num)
{
  mClientStatus[num].connected = false;
  mClientStatus[num].pinged = false;
}

void Websocket_connection::clientContact(int num)
{
  mClientStatus[num].lastContact = millis();
  mClientStatus[num].pinged = false;
}

bool Websocket_connection::handleMaxClients(int num)
{
  if (num == WEBSOCKETS_SERVER_CLIENT_MAX-1)
  {
    evictClient(num);
    return false;
  }
  return true;
}

void Websocket_connection::onDisable()
{

}
