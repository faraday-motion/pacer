#ifndef MQTT_CONNECTION_H
#define MQTT_CONNECTION_H

#include <WiFi.h>
#include <PubSubClient.h>
#include "../../configuration/configurator.h"
#include "./mqtt_connection_config.h"
#include "../base/base.hpp"

//Connection_module

class Mqtt_connection : public virtual Connection_module
{
private:
  IFMV * mFMV = nullptr;
  Mqtt_connection_config * mCfg = nullptr;
  WiFiClient mClient;
  PubSubClient * pMqttClient;
  void reconnect() ;
  void callback(char * topic, uint8_t * payload, unsigned int length);
protected:
  void onEvent(byte eventId, bool always = false)
  {
    mFMV -> moduleEvent(this, eventId);
  }
public:
  Mqtt_connection(byte id, IFMV * fmv, Mqtt_connection_config * cfg = nullptr) : Connection_module(id, Modules::MQTT_CONNECTION)  {
    mFMV = fmv;
    if (cfg == nullptr)
      mCfg = static_cast<Mqtt_connection_config*>(Configurator::Instance().createConfig(id, Configurations::MQTT_CONNECTION_CONFIG));
    else
      mCfg = cfg;
    setConfig();
  }

  enum Commands : byte {
    CONNECT
  };

  enum Events : byte {
    CONFIGURE
  };

  void setConfig()
  {
    setEnabled(mCfg -> enabled);
  }

  void setup();
  void loop();
  void command(byte command);
  void send(String message);
  
  String getModuleName()
  {
    return FPSTR("MQTT_CONNECTION");
  }
};

#endif
