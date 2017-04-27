#ifndef Wifi_h
#define Wifi_h

#include <IPAddress.h>
#include <ESP8266WiFi.h>
class Wifi {

private:

  byte ip[4];
  byte subnet[4];
  byte channel;

  int  port;
  const char *ssid;
  const char *password;

public:
  WiFiServer* server;
  WiFiClient  client;

  Wifi();
  void setup(WiFiServer* wifiServer);
  void registerClient();
  byte getChecksum(byte* array, byte arraySize);
  bool validateChecksum(byte* array, byte arra0ySize);
};

#endif
