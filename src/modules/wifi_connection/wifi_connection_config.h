#ifndef WIFI_CONNECTION_CONFIG_H
#define WIFI_CONNECTION_CONFIG_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../../configuration/default/configuration.h"
#include "../../configuration/default/default_pins.h"
#include "../../configuration/base/configbase.h"
#include "../../enums/enums.hpp"

class Wifi_connection_config : public Configbase{
public:
  Wifi_connection_config(byte id) : Configbase(id, Configurations::WIFI_CONNECTION_CONFIG) {
  }

  void getConfiguration(JsonObject &root)
  {
    root["id"] = id;
    root["configuration"] = (int)configuration;
    root["host"] = host;
    root["ssid"] = ssid;
    root["password"] = password;
    root["ap_ssid"] = ap_ssid;
    root["ap_password"] = ap_password;
    root["ap_channel"] = ap_channel;
  }

  void setConfiguration(JsonObject &root)
  {
    id = byte(root["id"]);
    host = root["host"].as<String>();
    ssid = root["ssid"].as<String>();
    password = root["password"].as<String>();
    ap_ssid = root["ap_ssid"].as<String>();
    ap_password = root["ap_password"].as<String>();
    ap_channel = int(root["ap_channel"]);
  }

  String host = WIFI_CONNECTION_HOST;
  String ssid = WIFI_CONNECTION_SSID;
  String password = WIFI_CONNECTION_PASSWORD;
  String ap_ssid = WIFI_CONNECTION_AP_SSID;
  String ap_password = WIFI_CONNECTION_AP_PASSWORD;
  int ap_channel = WIFI_CONNECTION_AP_CHANNEL;
};

#endif
