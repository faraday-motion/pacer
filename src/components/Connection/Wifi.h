#ifndef Wifi_h
#define Wifi_h

#include <IPAddress.h>
#include <ESP8266WiFi.h>
#include "../ConfigController/Config.h"
#include "../ConfigController/ConfigController.h"

class Wifi {

public:
  WiFiServer*  server;
  Config*      config;
  WiFiClient   client;

  Wifi();
  void setup(WiFiServer* wifiServer, ConfigController* configController);
  void registerClient();
  byte getChecksum(byte* array, byte arraySize);
  bool validateChecksum(byte* array, byte arra0ySize);
};

#endif
