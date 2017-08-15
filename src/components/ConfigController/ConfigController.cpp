#include "ConfigController.h"

ConfigController::ConfigController()
{
  this->config = new Config();
  configFilePath = "/config.json";
  factoryConfigPath = "/config_factory.json";

  if (this->loadConfig())
  {
    this->hasLoadedConfig = true;
  }
  else
  {
    this->restoreFactoryConfig();
    this->hasLoadedConfig = false;
  }

}

bool ConfigController::loadConfig()
{
  Serial.println("Loading configuration...");
  if(!this->beginSPIFFS()) {
    return false;
  }

  File configFile = this->getFile("r");
  if(!configFile){
    Serial.println("Failed to open the config file. Aborting");
    return false;
  }

  StaticJsonBuffer<2400> jsonBuffer;
  JsonObject& json = jsonBuffer.parse(configFile);

  if (!json.success()){
    SPIFFS.end();
    Serial.println("Failed to parse config file.");
    return false;
  }

  this->config->setConfig(json);
  configFile.close();
  SPIFFS.end();
  Serial.println("Configuratin loaded successfuly.");
  return true;
}

String ConfigController::getRawConfig(bool factoryConfig)
{
  Serial.println("Reading confinguration from file...");
  this->beginSPIFFS();

  File configFile = this->getFile("r", factoryConfig);
  if (configFile && configFile.size())
  {
    String rawConfig;
    while (configFile.available())
    {
      rawConfig += char(configFile.read());;
    }
    configFile.close();
    Serial.println("Configuration was read.");
    return rawConfig;
  }

  this->endSPIFFS();
}

bool ConfigController::writeRawConfig(String rawConfig)
{
  Serial.println("Writing new conifguration...");

  if(!this->beginSPIFFS()) {
    return false;
  }

  File configFile = ConfigController::getFile("w");
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    return false;
  }
  configFile.print(rawConfig);

  configFile.flush();
  configFile.close();
  this->endSPIFFS();

  Serial.println("Finished writing the new configuration. The ESP8266 will reboot now...");

  ESP.restart();

  return true;
}

File ConfigController::getFile(const char *permission, bool factory) {
   String path = this->configFilePath;
   if (factory == true)
   {
    path = this->factoryConfigPath;
   }
   if (!SPIFFS.exists(this->configFilePath))
   {
     Serial.print("File: '");
     Serial.print(this->factoryConfigPath);
     Serial.println("' does not exist.");
   }
   return SPIFFS.open(path, permission);
}


bool ConfigController::restoreFactoryConfig()
{
  Serial.println("::::::::::::::::::::::::::::::::::::::::::::::::::");
  Serial.println("                      PANIC                       ");
  Serial.println("::::::::::::::::::::::::::::::::::::::::::::::::::");
  Serial.println("Could not read the json config file.");
  Serial.println("Reverting to factory configuration...");

  String config = this->getRawConfig(true);
  bool result = this->writeRawConfig(config);
  yield();
  return true;
}


bool ConfigController::beginSPIFFS()
{
  if(!SPIFFS.begin()) {
    Serial.println("Error: Could not begin SPIFFS in ConfigController.cpp");
    return false;
  }
  return true;
}

bool ConfigController::endSPIFFS()
{
  SPIFFS.end();
}

// Debug
bool ConfigController::printConfig(JsonObject& json)
{
  json.printTo(Serial);
}
