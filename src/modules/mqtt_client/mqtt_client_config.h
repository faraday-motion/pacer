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

  void getConfiguration(JsonObject &root)
  {
    root["id"] = id;
    root["configuration"] = configuration;
    root["mqttServer"] = mqttServer;
    root["mqttPort"] = mqttPort;
    root["enabled"] = enabled;
  }

  void setConfiguration(JsonObject &root)
  {
    id = byte(root["id"]);
    mqttServer = root["mqttServer"].as<String>();;
    mqttPort = int(root["mqttPort"]);
    enabled = bool(root["enabled"]);
  }

  String mqttServer = MQTT_CLIENT_SERVER;
  int mqttPort = MQTT_CLIENT_PORT;
  bool enabled = MODULE_DEFAULT_ENABLED;
};

#endif
