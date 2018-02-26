#include <Arduino.h>
#include "./mqtt_client.h"
#include "../../configuration/default/configuration.h"

using namespace std;
using namespace std::placeholders;

//http://www.mqtt-dashboard.com/
//http://www.hivemq.com/demos/websocket-client/


void Mqtt_client::setup() {
  if (mIsSetup == false)
  {
    mIsSetup = true;
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up "), getModuleName());
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    randomSeed(micros());
    pMqttClient = new PubSubClient(mClient);
    pMqttClient -> setServer(mMqttServer.c_str(), mMqttPort);
    //If setCallback line is failing, the pubsubclient needs to be modified
    pMqttClient -> setCallback(std::bind(&Mqtt_client::callback, this, _1, _2, _3));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up "), getModuleName());
  }
}

void Mqtt_client::loop()
{
  if (enabled())
  {
    Logger::Instance().write(LogLevel::DEBUG, getModuleName(), FPSTR("::loop"));
    if (!pMqttClient -> connected()) {
      reconnect();
    }
    pMqttClient -> loop();

//    client.publish("outTopic", msg);
  }
}

void Mqtt_client::callback(char* topic, uint8_t* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void Mqtt_client::reconnect() {
  // Loop until we're reconnected
  while (!pMqttClient -> connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (pMqttClient -> connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      pMqttClient -> publish("outTopic", "hello world");
      // ... and resubscribe
      pMqttClient -> subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(pMqttClient -> state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void Mqtt_client::command(byte command)
{

}
