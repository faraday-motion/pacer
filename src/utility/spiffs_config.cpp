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

bool Spiffs_config::load(String path, byte &id, int &configuration) {
  Logger::Instance().write(LogLevel::DEBUG, FPSTR("load: "), path);
  File file;
  bool success = pStore -> read(path, file);
  if (!success)
    return false;
  StaticJsonBuffer<JSONBUFFERSIZE> jsonBuffer;
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

bool Spiffs_config::load(IConfig * config) {
  String path = "/configs/config" + Tools::prefixInt(config -> getId()) +  ".json";
  Logger::Instance().write(LogLevel::INFO, FPSTR("loadConfig: "), path);
  File file;
  bool success = pStore -> read(path, file);
  if (!success)
    return false;
  StaticJsonBuffer<JSONBUFFERSIZE> jsonBuffer;
  JsonObject &root = jsonBuffer.parseObject(file);
  if (!root.success())
    Logger::Instance().write(LogLevel::WARNING, FPSTR("Failed to read config: "), path);
  else
    config -> setConfiguration(root);
  file.close();
}

bool Spiffs_config::save(IConfig * config){
    String path = "/configs/config" + Tools::prefixInt(config -> getId()) +  ".json";
    Logger::Instance().write(LogLevel::INFO, FPSTR("saveConfig: "), path);
    StaticJsonBuffer<JSONBUFFERSIZE> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    config -> getConfiguration(root);
    String configContent;
    if (root.printTo(configContent) == 0) {
      Logger::Instance().write(LogLevel::WARNING, FPSTR("Failed to write to file: "), path);
      return false;
    }
    else
    {
      return pStore -> save(path, configContent);
    }
}

bool Spiffs_config::list(std::vector<String> &files){
    String path = "/configs/";
    Logger::Instance().write(LogLevel::DEBUG, FPSTR("Listing directory: "), path);
    File configDirectory;
    bool success = pStore -> read(path, configDirectory);
    if (!success)
      return false;
    if(!configDirectory.isDirectory()){
        Logger::Instance().write(LogLevel::WARNING, FPSTR("Not a directory: "), path);
        return false;
    }
    File file = configDirectory.openNextFile();
    while(file){
        Logger::Instance().write(LogLevel::DEBUG, FPSTR("FILE: "), String(file.name()));
        files.push_back(file.name());
        file.close();
        file = configDirectory.openNextFile();
    }
    return true;
}

bool Spiffs_config::remove(byte id){
  String path = "/configs/config" + Tools::prefixInt(id) +  ".json";
  return  pStore -> remove(path);
}

bool Spiffs_config::remove()
{
  std::vector<String> files;
  list(files);
  bool success = false;
  for (int i=0; i<files.size(); i++)
  {
    success = pStore -> remove(files[i]);
    if (!success)
      return false;
  }
  return true;
}
