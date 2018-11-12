#include <WebServer.h>
#include <Update.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>
#include <Arduino.h>
#include "./web_update.h"
#include "../../configuration/default/configuration.h"

//https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js
const char* updateIndex PROGMEM = "<script async src='/jquery-3.3.1.min.js'></script>"
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
    mIsSetup = true;
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up "), getModuleName());
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    onEvent(Events::CONFIGURE);
    Configurator::Instance().initializeSpiff();
    mWebserver = new WebServer(80);

    mWebserver -> on("/firmware", HTTP_GET, [this](){
      this -> mWebserver -> sendHeader("Connection", "close");
      this -> mWebserver -> send(200, "text/html", updateIndex);
    });
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

    //Load the default page
    mWebserver -> on("/", HTTP_GET, [this](){
      this -> handleFileRead("/");
    });
    //list directory
    mWebserver -> on("/list", HTTP_GET, [this](){
      this -> handleFileList();
    });

    //load editor
    mWebserver -> on("/edit", HTTP_GET, [this]() {
      if (!this -> handleFileRead("/edit.htm")) {
        mWebserver -> send(404, "text/plain", "FileNotFound");
      }
    });

    //create file
    mWebserver -> on("/edit", HTTP_PUT, [this]() {
      this -> handleFileCreate();
    });

    //delete file
    mWebserver -> on("/edit", HTTP_DELETE, [this]() {
      this -> handleFileDelete();
    });

    //first callback is called after the request has ended with all parsed arguments
    //second callback handles file uploads at that location
    mWebserver -> on("/edit", HTTP_POST, [this]() {
      this -> mWebserver -> send(200, "text/plain", "");
    }, [this]() {
      this -> handleFileUpload();
    });

    //called when the url is not defined here
    //use it to load content from SPIFFS
    mWebserver -> onNotFound([this]() {
      if (!this -> handleFileRead(this -> mWebserver -> uri())) {
        this -> mWebserver -> send(404, "text/plain", "FileNotFound");
      }
    });

    mWebserver -> begin();
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up "), getModuleName());
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

#define DBG_OUTPUT_PORT Serial
File fsUploadFile;

//format bytes
String Web_update::formatBytes(size_t bytes) {
  if (bytes < 1024) {
    return String(bytes) + "B";
  } else if (bytes < (1024 * 1024)) {
    return String(bytes / 1024.0) + "KB";
  } else if (bytes < (1024 * 1024 * 1024)) {
    return String(bytes / 1024.0 / 1024.0) + "MB";
  } else {
    return String(bytes / 1024.0 / 1024.0 / 1024.0) + "GB";
  }
}

String Web_update::getContentType(String filename) {
  if (mWebserver -> hasArg("download")) {
    return "application/octet-stream";
  } else if (filename.endsWith(".htm")) {
    return "text/html";
  } else if (filename.endsWith(".html")) {
    return "text/html";
  } else if (filename.endsWith(".css")) {
    return "text/css";
  } else if (filename.endsWith(".js")) {
    return "application/javascript";
  } else if (filename.endsWith(".png")) {
    return "image/png";
  } else if (filename.endsWith(".gif")) {
    return "image/gif";
  } else if (filename.endsWith(".jpg")) {
    return "image/jpeg";
  } else if (filename.endsWith(".ico")) {
    return "image/x-icon";
  } else if (filename.endsWith(".xml")) {
    return "text/xml";
  } else if (filename.endsWith(".pdf")) {
    return "application/x-pdf";
  } else if (filename.endsWith(".zip")) {
    return "application/x-zip";
  } else if (filename.endsWith(".gz")) {
    return "application/x-gzip";
  }
  return "text/plain";
}

bool Web_update::handleFileRead(String path) {
  DBG_OUTPUT_PORT.println("handleFileRead: " + path);
  if (path.endsWith("/")) {
    path += "index.html";
  }

  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if (mSpiffs_storage.exists(pathWithGz) || mSpiffs_storage.exists(path)) {
    if (mSpiffs_storage.exists(pathWithGz)) {
      path += ".gz";
    }
    Logger::Instance().write(LogLevel::INFO, FPSTR("mSpiffs_storage.read: "), path);
    File file;
    mSpiffs_storage.read(path, file);
    //File file = SPIFFS.open(path, "r");
    mWebserver -> streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}

void Web_update::handleFileUpload() {
  if (mWebserver -> uri() != "/edit") {
    return;
  }
  HTTPUpload& upload = mWebserver -> upload();
  if (upload.status == UPLOAD_FILE_START) {
    String filename = upload.filename;
    if (!filename.startsWith("/")) {
      filename = "/" + filename;
    }
    DBG_OUTPUT_PORT.print("handleFileUpload Name: "); DBG_OUTPUT_PORT.println(filename);
    fsUploadFile = SPIFFS.open(filename, "w");
    filename = String();
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    //DBG_OUTPUT_PORT.print("handleFileUpload Data: "); DBG_OUTPUT_PORT.println(upload.currentSize);
    if (fsUploadFile) {
      fsUploadFile.write(upload.buf, upload.currentSize);
    }
  } else if (upload.status == UPLOAD_FILE_END) {
    if (fsUploadFile) {
      fsUploadFile.close();
    }
    DBG_OUTPUT_PORT.print("handleFileUpload Size: "); DBG_OUTPUT_PORT.println(upload.totalSize);
  }
}

void Web_update::handleFileDelete() {
  if (mWebserver -> args() == 0) {
    return mWebserver -> send(500, "text/plain", "BAD ARGS");
  }
  String path = mWebserver -> arg(0);
  DBG_OUTPUT_PORT.println("handleFileDelete: " + path);
  if (path == "/") {
    return mWebserver -> send(500, "text/plain", "BAD PATH");
  }
  if (!mSpiffs_storage.exists(path)) {
    return mWebserver -> send(404, "text/plain", "FileNotFound");
  }
  SPIFFS.remove(path);
  mWebserver -> send(200, "text/plain", "");
  path = String();
}

void Web_update::handleFileCreate() {
  if (mWebserver -> args() == 0) {
    return mWebserver -> send(500, "text/plain", "BAD ARGS");
  }
  String path = mWebserver -> arg(0);
  DBG_OUTPUT_PORT.println("handleFileCreate: " + path);
  if (path == "/") {
    return mWebserver -> send(500, "text/plain", "BAD PATH");
  }
  if (mSpiffs_storage.exists(path)) {
    return mWebserver -> send(500, "text/plain", "FILE EXISTS");
  }
  File file = SPIFFS.open(path, "w");
  if (file) {
    file.close();
  } else {
    return mWebserver -> send(500, "text/plain", "CREATE FAILED");
  }
  mWebserver -> send(200, "text/plain", "");
  path = String();
}

void Web_update::handleFileList() {
  if (!mWebserver -> hasArg("dir")) {
    mWebserver -> send(500, "text/plain", "BAD ARGS");
    return;
  }

  String path = mWebserver -> arg("dir");
  DBG_OUTPUT_PORT.println("handleFileList: " + path);

  File root = SPIFFS.open(path);
  path = String();

  String output = "[";
  if(root.isDirectory()){
      File file = root.openNextFile();
      while(file){
          if (output != "[") {
            output += ',';
          }
          output += "{\"type\":\"";
          output += (file.isDirectory()) ? "dir" : "file";
          output += "\",\"name\":\"";
          output += String(file.name()).substring(1);
          output += "\"}";
          file = root.openNextFile();
      }
  }
  output += "]";
  mWebserver -> send(200, "text/json", output);
}
