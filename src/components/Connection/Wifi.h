#ifndef Wifi_h
#define Wifi_h

#include <IPAddress.h>
#include <ESP8266WiFi.h>
#include <Metro.h>
#include "../ConfigController/Config.h"
#include "../ConfigController/ConfigController.h"
#include "components/Connection/RadioDevice.h"
class Wifi {

private:
  Metro* handleClientInterval;

public:
  WiFiServer*  server;
  Config*      config;
  WiFiClient   client;

  Wifi();
  void setup(ConfigController* configController);
  bool handleClientConnections();

  RadioDevice pendingDevice;
  void clearPendingDevice();

};

#endif
