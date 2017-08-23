#include "WebServer.h"
#include <ESP8266WebServer.h>
#include "components/Utility/Log.h"
WebServer::WebServer()
{

}

void WebServer::setup()
{
  Log::Logger()->write(Log::Level::INFO, "Setting up webServer");
  this->webServer = new ESP8266WebServer(80);
  httpUpdater.setup(this->webServer, update_path, update_username, update_password);
  this->webServer->begin();
  Log::Logger()->write(Log::Level::INFO, "Finished setting up webServer");
}


void WebServer::handleClient()
{
  this->webServer->handleClient();
}
