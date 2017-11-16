#include "ConfigController.h"
#include "components/Utility/Log.h"

ConfigController::ConfigController()
{
  this->config = Config::get();
  
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
  Log::Logger()->write(Log::Level::INFO, "Loading configuration...");
  if(!this->beginSPIFFS()) {
    return false;
  }

  File configFile = this->getFile("r");
  if(!configFile){
    Log::Logger()->write(Log::Level::ERR, "Failed to open the config file. Aborting");
    return false;
  }

  Log::Logger()->write(Log::Level::DEBUG, "File Size: ");
  Log::Logger()->write(Log::Level::DEBUG, (String)configFile.size());
  StaticJsonBuffer<2400> jsonBuffer;
  JsonObject& json = jsonBuffer.parse(configFile);

  if (!json.success()){
    SPIFFS.end();
    Log::Logger()->write(Log::Level::ERR, "Failed to parse config file.");
    return false;
  }

  this->config->setConfig(json);
  configFile.close();
  SPIFFS.end();
  Log::Logger()->write(Log::Level::INFO, "Configuratin loaded successfuly.");
  return true;
}

String ConfigController::getRawConfig(bool factoryConfig)
{
  Log::Logger()->write(Log::Level::DEBUG, "Reading confinguration from file...");
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
    return rawConfig;
  }

  this->endSPIFFS();
}

bool ConfigController::writeRawConfig(String rawConfig)
{
  Log::Logger()->write(Log::Level::DEBUG, "Writing new conifguration...");
  if(!this->beginSPIFFS()) {
    Log::Logger()->write(Log::Level::ERR, "COULD NOT BEGIN SPIFFS");
    return false;
  }

  File configFile = ConfigController::getFile("w");
  if (!configFile) {
    Log::Logger()->write(Log::Level::ERR, "Failed to open config file for writing");
    return false;
  }
  
  // Write config to file.
  configFile.print(rawConfig);
  configFile.flush();
  configFile.close();

  this->endSPIFFS();

  Log::Logger()->write(Log::Level::DEBUG, "Finished writing the new configuration. The ESP8266 will reboot now...");

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
     Log::Logger()->write(Log::Level::ERR, "File: ''" + this->factoryConfigPath + "'  does not exist." );
   }
   return SPIFFS.open(path, permission);
}


bool ConfigController::restoreFactoryConfig()
{

  Log::Logger()->write(Log::Level::ERR, "::::::::::::::::::::::::::::::::::::::::::::::::::" );
  Log::Logger()->write(Log::Level::ERR, "                      PANIC                       " );
  Log::Logger()->write(Log::Level::ERR, "::::::::::::::::::::::::::::::::::::::::::::::::::" );
  Log::Logger()->write(Log::Level::ERR, "Could not read the json config file: ");
  Log::Logger()->write(Log::Level::ERR, "Reverting to factory configuration..." );  
  String config = this->getRawConfig(true);
  bool result = this->writeRawConfig(config);
  yield();
  return true;
}


bool ConfigController::beginSPIFFS()
{
  if(!SPIFFS.begin()) {
      Log::Logger()->write(Log::Level::ERR, "Error: Could not begin SPIFFS in ConfigController.cpp" );
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
