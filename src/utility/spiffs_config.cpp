#include "./spiffs_config.h"
#include <vector>
#include <Arduino.h>
#include <ArduinoJson.h>
#include "SPIFFS.h"
#include "../configuration/default/configuration.h"
#include "../configuration/base/configbase.h"
#include "../configuration/configurator.h"
#include "../log/logger.h"
#include "../utility/tools.h"

void Spiffs_config::loadBase(String path, byte &id, int &configuration) {
  Logger::Instance().write(LogLevel::DEBUG, FPSTR("loadConfigBase: "), path);
  File file = fs.open(path);
  if(!file || file.isDirectory()){
      Logger::Instance().write(LogLevel::WARNING, FPSTR("Failed to open config for reading: "), path);
      return ;
  }
  // Allocate the memory pool on the stack.
  // Don't forget to change the capacity to match your JSON document.
  // Use arduinojson.org/assistant to compute the capacity.
  StaticJsonBuffer<JSONBUFFERSIZE> jsonBuffer;
  // Parse the root object
  JsonObject &root = jsonBuffer.parseObject(file);
  if (!root.success())
    Logger::Instance().write(LogLevel::WARNING, FPSTR("Failed to read config: "), path);
  else
  {
    id = int(root["id"]);
    configuration = int(root["configuration"]);
  }
  file.close();
}

void Spiffs_config::load(Configbase *config) {
  String path = "/config" + Tools::prefixInt(config -> id) +  ".json";
  Logger::Instance().write(LogLevel::INFO, FPSTR("loadConfig: "), path);
  File file = fs.open(path);
  if(!file || file.isDirectory()){
    Logger::Instance().write(LogLevel::WARNING, FPSTR("Failed to open file for reading: "), path);
    return ;
  }
  // Allocate the memory pool on the stack.
  // Don't forget to change the capacity to match your JSON document.
  // Use arduinojson.org/assistant to compute the capacity.
  StaticJsonBuffer<JSONBUFFERSIZE> jsonBuffer;
  // Parse the root object
  JsonObject &root = jsonBuffer.parseObject(file);
  if (!root.success())
    Logger::Instance().write(LogLevel::WARNING, FPSTR("Failed to read config: "), path);
  else
    config -> setConfiguration(root);
  file.close();
}

bool Spiffs_config::save(Configbase *config){
    String path = "/config" + Tools::prefixInt(config -> id) +  ".json";
    Logger::Instance().write(LogLevel::INFO, FPSTR("saveConfig: "), path);
    //deleteFile(path);
    File file = fs.open(path, FILE_WRITE);
    if(!file){
      Logger::Instance().write(LogLevel::WARNING, FPSTR("Failed open for write: "), path);
      return false;
    }
    // Serialize JSON to file
    StaticJsonBuffer<JSONBUFFERSIZE> jsonBuffer;
    // Parse the root object
    JsonObject &root = jsonBuffer.createObject();
    config -> getConfiguration(root);
    bool result = false;
    if (root.printTo(file) == 0) {
      Logger::Instance().write(LogLevel::WARNING, FPSTR("Failed to write to file: "), path);
    }
    else
    {
      Logger::Instance().write(LogLevel::DEBUG, FPSTR("File write success: "), path);
      result =  true;
    }
    file.close();
}

std::vector<String> Spiffs_config::list(){
    String path = "/";
    Logger::Instance().write(LogLevel::DEBUG, FPSTR("Listing directory: "), path);
    std::vector<String> files;
    File root = fs.open(path);
    if(!root){
      Logger::Instance().write(LogLevel::WARNING, FPSTR("Failed to open directory: "), path);
      return files;
    }
    if(!root.isDirectory()){
        Logger::Instance().write(LogLevel::WARNING, FPSTR("Not a directory: "), path);
        return files;
    }
    File file = root.openNextFile();
    while(file){
        Logger::Instance().write(LogLevel::DEBUG, FPSTR("FILE: "), String(file.name()));
        files.push_back(file.name());
        file.close();
        file = root.openNextFile();
    }
    return files;
}

bool Spiffs_config::remove(byte id){
  String path = "/config" + Tools::prefixInt(id) +  ".json";
  return deleteFile(path);
}

void Spiffs_config::removeAll()
{
  std::vector<String> files = list();
  for (int i=0; i<files.size(); i++)
    deleteFile(files[i]);
}

bool Spiffs_config::deleteFile(String path){
  Logger::Instance().write(LogLevel::DEBUG, FPSTR("Deleting file: "), path);
  if(fs.remove(path)){
    Logger::Instance().write(LogLevel::DEBUG, FPSTR("File deleted"));
    return true;
  } else {
    Logger::Instance().write(LogLevel::WARNING, FPSTR("Delete failed"));
    return false;
  }
}
