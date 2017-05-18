#ifndef Wifi_h
#define Wifi_h

#include <IPAddress.h>
#include <ESP8266WiFi.h>
#include <Metro.h>
#include "../ConfigController/Config.h"
#include "../ConfigController/ConfigController.h"

class Wifi {
private:
  Metro* handleClientInterval;
public:
  WiFiServer*  server;
  Config*      config;
  WiFiClient   client;

  Wifi();
  void setup(ConfigController* configController);
  void handleClientConnections();

};

#endif
