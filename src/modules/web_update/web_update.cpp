#include <ESP32WebServer.h>
#include <Update.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>
#include <Arduino.h>
#include "./web_update.h"
#include "../../configuration/default/configuration.h"

const char* serverIndex PROGMEM = "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
"<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
    "<input type='file' name='update'>"
    "<input type='submit' value='Update'>"
"</form>"
"<div id='prg'>progress: 0%</div>"
"<script>"
"$('form').submit(function(e){"
    "e.preventDefault();"
      "var form = $('#upload_form')[0];"
      "var data = new FormData(form);"
      " $.ajax({"
            "url: '/update',"
            "type: 'POST',"
            "data: data,"
            "contentType: false,"
            "processData:false,"
            "xhr: function() {"
                "var xhr = new window.XMLHttpRequest();"
                "xhr.upload.addEventListener('progress', function(evt) {"
                    "if (evt.lengthComputable) {"
                        "var per = evt.loaded / evt.total;"
                        "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
                    "}"
               "}, false);"
               "return xhr;"
            "},"
            "success:function(d, s) {"
                "console.log('success!')"
           "},"
            "error: function (a, b, c) {"
            "}"
          "});"
"});"
"</script>";


void Web_update::setup() {
  if (mIsSetup == false)
  {
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up Web_update"));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    Configurator::Instance().initializeSpiff();
    mWebserver = new ESP32WebServer(80);
    /*return index page which is stored in serverIndex */
    mWebserver -> on("/", HTTP_GET, [this](){
      this -> mWebserver -> sendHeader("Connection", "close");
      this -> mWebserver -> send(200, "text/html", serverIndex);
    });
    /*handling uploading firmware file */
    mWebserver -> on("/update", HTTP_POST, [this](){
      mWebserver -> sendHeader("Connection", "close");
      mWebserver -> send(200, "text/plain", (Update.hasError())?"FAIL":"OK");
      ESP.restart();
    },[this](){
      HTTPUpload& upload = this -> mWebserver -> upload();
      if(upload.status == UPLOAD_FILE_START){
        Serial.printf("Update: %s\n", upload.filename.c_str());
        if(!Update.begin(UPDATE_SIZE_UNKNOWN)){//start with max available size
          Update.printError(Serial);
        }
      } else if(upload.status == UPLOAD_FILE_WRITE){
        /* flashing firmware to ESP*/
        if(Update.write(upload.buf, upload.currentSize) != upload.currentSize){
          Update.printError(Serial);
        }
      } else if(upload.status == UPLOAD_FILE_END){
        if(Update.end(true)){ //true to set the size to the current progress
          Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
        } else {
          Update.printError(Serial);
        }
      }
    });
    mWebserver -> begin();
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up Web_update"));
    mIsSetup = true;
  }
}

void Web_update::loop()
{
  if (enabled())
  {
    if (mSimpleTimer.check())
    {
      Logger::Instance().write(LogLevel::DEBUG, FPSTR("Web_update::loop"));
      mWebserver -> handleClient();
    }
  }
}

void Web_update::command(byte command)
{

}


void Web_update::setRoutes()
{
  /*
  // Handle GET Routes
  mWebserver->on("/", HTTP_GET, [this]() {
    handleFileStream("/");
  });
  mWebserver->on("/index.html", HTTP_GET, [this]() {
    handleFileStream("/index.html");
  });
  mWebserver->on("/console.html", HTTP_GET, [this]() {
    handleFileStream("/console.html");
  });
  //Handle Assets
  mWebserver->on("/style.css", HTTP_GET, [this]() {
    handleFileStream("/style.css");
  });
  mWebserver->on("/websocket.js", HTTP_GET, [this]() {
    handleFileStream("/websocket.js");
  });
  mWebserver->on("/console.js", HTTP_GET, [this]() {
    handleFileStream("/console.js");
  });
  mWebserver->on("/setup.js", HTTP_GET, [this]() {
    handleFileStream("/setup.js");
  });
  mWebserver->on("/default_config.js", HTTP_GET, [this]() {
    handleFileStream("/default_config.js");
  });
  */
}

// Streaming files to the webserver clients.
bool Web_update::handleFileStream(String path)
{
  //Log::Logger()->write(Log::Level::DEBUG, "Webserver trying to stream file.");
  path = "/web" + path;
  if(path.endsWith("/")) path += "index.html";
  String contentType = getContentType(path);
  // TODO:: Convert files to .gz;
  String pathWithGz = path + ".gz";
//  SPIFFS.begin();
  //Log::Logger()->write(Log::Level::DEBUG, "Looking for file '" + path + "'");
  if (SPIFFS.exists(path) || SPIFFS.exists(pathWithGz)) {
    if(SPIFFS.exists(pathWithGz))
      path += ".gz";
    File file = SPIFFS.open(path, "r");
    size_t sent = mWebserver->streamFile(file, contentType);
    file.close();
    //Log::Logger()->write(Log::Level::DEBUG,"Webserver served file succesfully.");
//    SPIFFS.end();
    return true;
  }
//  Log::Logger()->write(Log::Level::DEBUG,"Webserver failed to stream file.");
  return false;
}


// Getting the file type (MIME)
String Web_update::getContentType(String filename) {
  if(mWebserver->hasArg("download")) return "application/octet-stream";
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
