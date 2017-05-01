#include "ConfigController.h"
#include <Arduino.h>
#include "ArduinoJson.h"
#include "FS.h"

ConfigController::ConfigController(Config* config)
{
  this->config = config;
  configFilePath = "/config.json";
  factoryConfigPath = "/config_factory.json";
}

bool ConfigController::loadConfig()
{
  ConfigController::getJsonConfig();

  StaticJsonBuffer<1024> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(configString);

  if (!json.success()){
    Serial.println("Failed to parse config file.");
    return false;
  }

  config->wifi.ssid      = json["wifi"]["ssid"];
  config->wifi.port      = json["wifi"]["port"];
  config->wifi.channel   = json["wifi"]["channel"];
  config->wifi.password  = json["wifi"]["password"];
  config->wifi.ip[0]     = json["wifi"]["ip"][0];
  config->wifi.ip[1]     = json["wifi"]["ip"][1];
  config->wifi.ip[2]     = json["wifi"]["ip"][2];
  config->wifi.ip[3]     = json["wifi"]["ip"][3];
  config->wifi.subnet[0] = json["wifi"]["subnet"][0];
  config->wifi.subnet[1] = json["wifi"]["subnet"][1];
  config->wifi.subnet[2] = json["wifi"]["subnet"][2];
  config->wifi.subnet[3] = json["wifi"]["subnet"][3];

  // controller
  config->controller.defaultSmoothAlpha   = json["controller"]["defaultSmoothAlpha"];
  config->controller.defaultInputNeutral  = json["controller"]["defaultInputNeutral"];
  config->controller.defaultInputMinBrake = json["controller"]["defaultInputMinBrake"];
  config->controller.defaultInputMaxBrake = json["controller"]["defaultInputMaxBrake"];
  config->controller.defaultInputMinAcceleration = json["controller"]["defaultInputMinAcceleration"];
  config->controller.defaultInputMaxAcceleration = json["controller"]["defaultInputMaxAcceleration"];

  // currentControl
  config->currentControl.defaultCurrentNeutral  = json["currentControl"]["defaultCurrentNeutral"];
  config->currentControl.defaultCurrentBrakeMin = json["currentControl"]["defaultCurrentBrakeMin"];
  config->currentControl.defaultCurrentBrakeMax = json["currentControl"]["defaultCurrentBrakeMax"];
  config->currentControl.defaultCurrentAccelerationMin = json["currentControl"]["defaultCurrentAccelerationMin"];
  config->currentControl.defaultCurrentAccelerationMax = json["currentControl"]["defaultCurrentAccelerationMax"];

  return true;
}


// Get a json string from the phone app and set it as the current stringConfig
//bool ConfigController::setConfigString (char *newConfigString)
bool ConfigController::setConfigString(String newConfigString)
{
  configString = newConfigString;
  return true;
}


bool ConfigController::saveConfig()
{
  StaticJsonBuffer<1024> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(configString);

  if(!json.success()) {
    Serial.println("The new configuration JSON is not valid. The configurarion was not saved.");
    return false;
  }

  File configFileUpdate = ConfigController::getFile("w");
  if (!configFileUpdate) {
    Serial.println("Failed to open config file for writing");
    return false;
  }
  // Write the new config to the configFile.
  json.printTo(configFileUpdate);
  configFileUpdate.close();

  return true;
}


bool ConfigController::getJsonConfig()
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

  json.printTo(configString);
  configFile.close();


  return true;
}


File ConfigController::getFile(const char *permission) {
   return SPIFFS.open(configFilePath, permission);
}
