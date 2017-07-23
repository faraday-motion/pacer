#ifndef Wifi_h
#define Wifi_h

#include <IPAddress.h>
#include <ESP8266WiFi.h>
#include <Metro.h>
#include "components/ConfigController/Config.h"
#include "components/ConfigController/ConfigController.h"
#include "components/Device/WifiDevice.h"
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

  WifiDevice pendingDevice;
  void clearPendingDevice();

};

#endif
