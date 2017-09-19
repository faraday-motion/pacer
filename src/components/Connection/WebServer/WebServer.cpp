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
  this->setRoutes();
  // Start webserver
  this->webServer->begin();
  Log::Logger()->write(Log::Level::INFO, "Finished setting up webServer");
}

void WebServer::handleClient()
{
  this->webServer->handleClient();
}


// Private functions

void WebServer::setRoutes()
{
  // Handle GET Routes
  this->webServer->on("/", HTTP_GET, [this]() {
    this->handleFileStream("/");
  });

  this->webServer->on("/index.html", HTTP_GET, [this]() {
    this->handleFileStream("/index.html");
  });

  this->webServer->on("/console.html", HTTP_GET, [this]() {
    this->handleFileStream("/console.html");
  });

  //Handle Assets
  this->webServer->on("/style.css", HTTP_GET, [this]() {
    this->handleFileStream("/style.css");
  });

  this->webServer->on("/websocket.js", HTTP_GET, [this]() {
    this->handleFileStream("/websocket.js");
  });

  this->webServer->on("/console.js", HTTP_GET, [this]() {
    this->handleFileStream("/console.js");
  });

  this->webServer->on("/setup.js", HTTP_GET, [this]() {
    this->handleFileStream("/setup.js");
  });

  this->webServer->on("/default_config.js", HTTP_GET, [this]() {
    this->handleFileStream("/default_config.js");
  });
}


// Streaming files to the webserver clients.
bool WebServer::handleFileStream(String path)
{

  Log::Logger()->write(Log::Level::DEBUG, "Webserver trying to stream file.");
  path = "/web" + path;
  if(path.endsWith("/")) path += "index.html";
  String contentType = this->getContentType(path);
  // TODO:: Convert files to .gz;
  String pathWithGz = path + ".gz";
  SPIFFS.begin();
  Log::Logger()->write(Log::Level::DEBUG, "Looking for file '" + path + "'");
  if (SPIFFS.exists(path) || SPIFFS.exists(pathWithGz)) {
    if(SPIFFS.exists(pathWithGz))
      path += ".gz";
    File file = SPIFFS.open(path, "r");
    size_t sent = this->webServer->streamFile(file, contentType);
    file.close();

    Log::Logger()->write(Log::Level::DEBUG,"Webserver served file succesfully.");
    SPIFFS.end();
    return true;
  }
  Log::Logger()->write(Log::Level::DEBUG,"Webserver failed to stream file.");

  return false;
}


// Getting the file type (MIME)
String WebServer::getContentType(String filename) {
  if(this->webServer->hasArg("download")) return "application/octet-stream";
  else if(filename.endsWith(".htm")) return "text/html";
  else if(filename.endsWith(".html")) return "text/html";
  else if(filename.endsWith(".css")) return "text/css";
  else if(filename.endsWith(".js")) return "application/javascript";
  else if(filename.endsWith(".png")) return "image/png";
  else if(filename.endsWith(".gif")) return "image/gif";
  else if(filename.endsWith(".jpg")) return "image/jpeg";
  else if(filename.endsWith(".ico")) return "image/x-icon";
  else if(filename.endsWith(".xml")) return "text/xml";
  else if(filename.endsWith(".pdf")) return "application/x-pdf";
  else if(filename.endsWith(".zip")) return "application/x-zip";
  else if(filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}
