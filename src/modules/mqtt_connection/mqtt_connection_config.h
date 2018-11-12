#ifndef MQTT_CONNECTION_CONFIG_H
#define MQTT_CONNECTION_CONFIG_H
#include <memory>
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../../configuration/default/configuration.h"
#include "../../configuration/default/default_pins.h"
#include "../../configuration/base/configbase.h"

class Mqtt_connection_config : public Configbase{
public:
  Mqtt_connection_config(byte id) : Configbase(id, Configurations::MQTT_CONNECTION_CONFIG) {
  }

  void getModuleConfiguration(JsonObject &root)
  {
    root["mqttServer"] = mqttServer;
    root["mqttPort"] = mqttPort;
  }

  void setModuleConfiguration(JsonObject &root)
  {
    mqttServer = root["mqttServer"].as<String>();;
    mqttPort = int(root["mqttPort"]);
  }

  String mqttServer = MQTT_CLIENT_SERVER;
  int mqttPort = MQTT_CLIENT_PORT;
};

#endif
