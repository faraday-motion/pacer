#ifndef MQTT_CLIENT_CONFIG_H
#define MQTT_CLIENT_CONFIG_H
#include <memory>
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../../configuration/default/configuration.h"
#include "../../configuration/default/default_pins.h"
#include "../../configuration/base/configbase.h"

class Mqtt_client_config : public Configbase{
public:
  Mqtt_client_config(byte id) : Configbase(id, Configurations::MQTT_CLIENT_CONFIG) {
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
