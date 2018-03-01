#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <WiFi.h>
#include <PubSubClient.h>
#include "../../configuration/configurator.h"
#include "./mqtt_client_config.h"
#include "../base/base.hpp"

class Mqtt_client : virtual public Modulebase
{
private:
  FMV * mFMV = nullptr;
  Mqtt_client_config * mCfg = nullptr;
  WiFiClient mClient;
  PubSubClient * pMqttClient;
  String mMqttServer = "";
  int mMqttPort = 0;
  void reconnect() ;
  void callback(char* topic, uint8_t* payload, unsigned int length);
protected:
  void onEvent(byte eventId, bool always = false)
  {
    mFMV -> moduleEvent(this, eventId);
  }
public:
  Mqtt_client(byte id, FMV *fmv, Mqtt_client_config* cfg = nullptr) : Modulebase(id, Modules::MQTT_CLIENT)  {
    mFMV = fmv;
    if (cfg == nullptr)
      mCfg = static_cast<Mqtt_client_config*>(Configurator::Instance().createConfig(id, Configurations::MQTT_CLIENT_CONFIG));
    else
      mCfg = cfg;
    setConfig();
  }

  void setConfig()
  {
    mMqttServer = mCfg -> mqttServer;
    mMqttPort = mCfg -> mqttPort;
    setEnabled(mCfg -> enabled);
  }

  void setup();
  void loop();
  void command(byte command);

  String getModuleName()
  {
    return FPSTR("MQTT_CLIENT");
  }
};

#endif
