#include "ConfigController.h"
#include <Arduino.h>
#include "ArduinoJson.h"
#include "FS.h"

ConfigController::ConfigController()
{
  this->config = new Config;
  configFilePath = "/config.json";
  factoryConfigPath = "/config_factory.json";
}

// Get a json string from the phone app and set it as the current stringConfig
bool ConfigController::setConfigString(String newConfigString)
{
  configString = newConfigString;
  return true;
}


bool ConfigController::loadConfig()
{

  if(!beginSPIFFS()) {
    return false;
  }

  ConfigController::getJsonConfig();

  StaticJsonBuffer<1820> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(configString);

  if (!json.success()){
    Serial.println("Failed to parse config file.");
    return false;
  }

  // Loop through all default wired devices.
  for (JsonObject::iterator it=json.begin(); it!=json.end(); ++it)
  {
    if(!strcmp(it->key, "wiredDevices")){
      byte length = it->value.size();
      config->wiredDevicesCount = length;
      for (size_t i = 0; i < length; i++) {
        config->wiredDevices[i].id = it->value[i]["id"];
        config->wiredDevices[i].type = it->value[i]["type"];
        config->wiredDevices[i].priority = it->value[i]["priority"];
        config->wiredDevices[i].enabled = it->value[i]["enabled"];
      }
    }
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

  endSPIFFS();
  return true;
}

bool ConfigController::saveConfig()
{
  if(!beginSPIFFS()) {
    return false;
  }

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
  endSPIFFS();

  return true;
}


bool ConfigController::getJsonConfig()
{
  if(!beginSPIFFS()) {
    return false;
  }

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
  endSPIFFS();
  return true;
}


File ConfigController::getFile(const char *permission) {
   return SPIFFS.open(configFilePath, permission);
}

bool ConfigController::beginSPIFFS()
{
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount File System");
    return false;
  }
  return true;
}
bool ConfigController::endSPIFFS()
{
  SPIFFS.end();
  return true;
}
