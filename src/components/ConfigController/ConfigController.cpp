#include "ConfigController.h"
#include <Arduino.h>
#include "ArduinoJson.h"
#include "FS.h"

ConfigController::ConfigController()
{
  configFilePath = "/config.json";
}

bool ConfigController::getConfig()
{
  //File configFile = ConfigController::getFile("r");
  File configFile = SPIFFS.open("/config.json", "r");
  if(!configFile) {
    Serial.println("Failed to open config file");
    return false;
  }

  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println("Config file size is too large");
    return false;
  }

  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);

  StaticJsonBuffer<1024> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());

  if (!json.success()){
    Serial.println("Failed to parse config file");
    return false;
  }

  const char* ssid = json["controller"]["defaultInputMaxAcceleration"];

  return true;
}

bool ConfigController::saveConfig()
{
  File configFile = ConfigController::getFile("r");
  if(!configFile) {
    Serial.println("Failed to open config file");
    return false;
  }

  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println("Config file size is too large");
    return false;
  }

  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);
  StaticJsonBuffer<1024> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());
  configFile.close();


  File configFileUpdate = ConfigController::getFile("w");
  if (!configFileUpdate) {
    Serial.println("Failed to open config file for writing");
    return false;
  }
  json.printTo(configFileUpdate);
  configFileUpdate.close();
  return true;
}


File ConfigController::getFile(const char *permission) {
   return SPIFFS.open(configFilePath, permission);
}
