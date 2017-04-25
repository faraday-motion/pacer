#ifndef Wifi_h
#define Wifi_h

#include <IPAddress.h>
#include <ESP8266WiFi.h>

class Wifi {

public:
  WiFiServer* server;
  WiFiClient client;
  byte MAX_SRV_CLIENTS;

  Wifi();
  void setup(WiFiServer* wifiServer);
  void registerClient();
  byte getChecksum(byte* array, byte arraySize);
  bool validateChecksum(byte* array, byte arra0ySize);
};

#endif
