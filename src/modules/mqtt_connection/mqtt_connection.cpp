#include <Arduino.h>
#include <cstddef>
#include <string>
#include "./mqtt_connection.h"
#include "../../configuration/default/configuration.h"

using namespace std;
using namespace std::placeholders;

//http://www.mqtt-dashboard.com/
//http://www.hivemq.com/demos/websocket-client/
//https://www.hivemq.com/blog/mqtt-essentials-part-8-retained-messages
//https://www.hivemq.com/blog/mqtt-essentials-part-5-mqtt-topics-best-practices

void Mqtt_connection::setup() {
  if (mIsSetup == false)
  {
    mIsSetup = true;
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up "), getModuleName());
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    onEvent(Events::CONFIGURE);

    randomSeed(micros());
    pMqttClient = new PubSubClient(mClient);
    pMqttClient -> setServer(mCfg -> mqttServer.c_str(), mCfg -> mqttPort);
    //If setCallback line is failing, the pubsubclient needs to be modified
    pMqttClient -> setCallback(std::bind(&Mqtt_connection::callback, this, _1, _2, _3));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up "), getModuleName());
  }
}

void Mqtt_connection::loop()
{
  if (enabled())
  {
    Logger::Instance().write(LogLevel::DEBUG, getModuleName(), FPSTR("::loop"));
    pMqttClient -> loop();
  }
}

void Mqtt_connection::command(byte command)
{
  if (enabled())
  {
    Logger::Instance().write(LogLevel::INFO, FPSTR("Mqtt_client::command: "), String(command));
    Commands comm = static_cast<Commands>(command);
    if (comm == Commands::CONNECT)
    {
      if (!pMqttClient -> connected()) {
        reconnect();
      }
    }
  }
}

void Mqtt_connection::callback(char * topic, uint8_t * payload, unsigned int length) {
  Logger::Instance().write(LogLevel::INFO, FPSTR("Mqtt_client::Message arrived on topic: "), String(topic));
  string st = topic;
  std::size_t found = st.find_last_of("/\\");
  string cmdString = st.substr(found + 1);
  Logger::Instance().write(LogLevel::INFO, FPSTR("Command: "), String(cmdString.c_str()));
  byte command = ExternalCommands::get(cmdString.c_str());
  Logger::Instance().write(LogLevel::INFO, FPSTR("Command: "), String(command));
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  byte value = std::atoi(message.c_str());
  //For each reciever do
  for (byte i=0; i < recievers().size(); i++) {
    Logger::Instance().write(LogLevel::DEBUG, FPSTR("Mqtt_connection recievers:"), String(i));
    recievers()[i] -> recieve(command, value);
  }
  send("OK");
  Logger::Instance().write(LogLevel::INFO, FPSTR("Value: "), message);
}

void Mqtt_connection::send(String message)
{
  if (enabled())
  {
    //Not allowed to add any log writes here as it creates a recursive loop.
    //message = "{\"response\":\"ok\"" + message + "}";
    if (pMqttClient -> connected())
      pMqttClient -> publish("outpacer", message.c_str());
  }
}

void Mqtt_connection::reconnect() {
  // Loop until we're reconnected
  byte maxRetries = 3;
  while (!pMqttClient -> connected() && maxRetries > 0) {
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (pMqttClient -> connect(clientId.c_str())) {
      Logger::Instance().write(LogLevel::DEBUG, FPSTR("Mqtt_client::connected"));
      pMqttClient -> publish("outpacer", "Connected!");
      pMqttClient -> subscribe("inpacer/commands/drive_power");
      pMqttClient -> subscribe("inpacer/commands/drive_brake");
      pMqttClient -> subscribe("inpacer/commands/turn_left");
      pMqttClient -> subscribe("inpacer/commands/turn_right");
      pMqttClient -> subscribe("inpacer/commands/led_enabled");
      pMqttClient -> subscribe("inpacer/commands/led_disabled");
    } else {
      Logger::Instance().write(LogLevel::DEBUG, FPSTR("Mqtt_client::not connected: "), String(pMqttClient -> state()));
      maxRetries --;
      delay(2500);
    }
  }
}
