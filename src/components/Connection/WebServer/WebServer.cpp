#include "WebServer.h"
#include <ESP8266WebServer.h>
WebServer::WebServer()
{

}

void WebServer::setup()
{
  this->webServer = new ESP8266WebServer(80);
  httpUpdater.setup(this->webServer, update_path, update_username, update_password);
  this->webServer->begin();
}


void WebServer::handleClient()
{
  this->webServer->handleClient();
}
