#include "ConfigController.h"

ConfigController::ConfigController()
{
  this->config = new Config();
  configFilePath = "/config.json";
  factoryConfigPath = "/config_factory.json";
}


bool ConfigController::loadConfig()
{

  if(!SPIFFS.begin()) {
    Serial.println("Error: Could not begin SPIFFS in ConfigController.cpp -> ConfigController:loadConfig()");
    return false;
  }

  File data = this->getFile("r");
  std::unique_ptr<char[]> buf(new char[2400]);
  data.readBytes(buf.get(), 2400);
  StaticJsonBuffer<2400> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());

  if (!json.success()){
    Serial.println("Failed to parse config file.");
    return false;
  }
  this->config->setConfig(json);

  SPIFFS.end();
  return true;
}

bool ConfigController::saveConfig()
{
  if(!SPIFFS.begin()) {
    Serial.println("Error: Could not begin SPIFFS in ConfigController.cpp -> ConfigController:saveConfig()");
    return false;
  }

  StaticJsonBuffer<1820> jsonBuffer;
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
  SPIFFS.end();

  return true;
}


File ConfigController::getFile(const char *permission) {
   return SPIFFS.open(configFilePath, permission);
}

// Debug
bool ConfigController::printConfig(JsonObject& json)
{
  json.printTo(Serial);
}
