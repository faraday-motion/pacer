#ifndef Wifi_h
#define Wifi_h

#include <Metro.h>
#include <Arduino.h>
#include <IPAddress.h>
#include <ESP8266WiFi.h>

#include "components/Config/Config.h"
#include "components/Device/WifiDevice.h"
#include "components/Config/Config.h"


class Wifi {

private:
  Metro* handleClientInterval;

public:

  WiFiServer*  server;
  Config*      config;
  WiFiClient   client;

  Wifi();
  void setup();
  bool handleClientConnections();

  WifiDevice pendingDevice;
  void clearPendingDevice();

};

#endif
