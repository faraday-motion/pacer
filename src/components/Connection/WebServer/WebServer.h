#ifndef WebServer_H
#define WebServer_H

#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>

class WebServer
{
public:
  ESP8266WebServer* webServer;
  ESP8266HTTPUpdateServer httpUpdater;
  const char* update_path = "/FMV_update";
  const char* update_username = "SUPER_ADMIN";
  const char* update_password = "admin";

  WebServer();
  void setup();
  void handleClient();
};


#endif
