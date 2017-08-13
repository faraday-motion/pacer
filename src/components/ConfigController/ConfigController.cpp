#include "ConfigController.h"

ConfigController::ConfigController()
{
  this->config = new Config();
  configFilePath = "/config.json";
  factoryConfigPath = "/config_factory.json";
  Serial.println("Using the following configuration");
  String rawConfig = this->getRawConfig();
  Serial.println(rawConfig);
  Serial.println("... end of configuration file");


  //TODO:: Implement code that copies config_factory.json into config.json upon error in cofig file.
  // SPIFFS.begin();
  // File configFile = this->getFile("w+");
  // String input = "{\"wifi\":{\"ssid\":\"FARADAY200\",\"port\":8899,\"ip\":[10,10,100,254],\"subnet\":[255,255,255,0],\"channel\":11,\"pass\":\"faraday200\"},\"websocket\":{\"port\":81}}";
  // configFile.print(input);
  // configFile.flush();
  // configFile.close();
  // SPIFFS.end();
}


bool ConfigController::loadConfig()
{

  if(!SPIFFS.begin()) {
    Serial.println("Error: Could not begin SPIFFS in ConfigController.cpp -> ConfigController:loadConfig()");
    return false;
  }

  File configFile = this->getFile("r");
  // NOTE:: We don't need to read from the file. ArduinoJson can parse the file.
  // std::unique_ptr<char[]> buf(new char[2400]);
  // configFile.readBytes(buf.get(), 2400);
  StaticJsonBuffer<2400> jsonBuffer;
  JsonObject& json = jsonBuffer.parse(configFile);

  if (!json.success()){
    SPIFFS.end();
    Serial.println("Failed to parse config file. Rewrote the defaults.");
    return false;
  }

  this->config->setConfig(json);
  configFile.close();
  SPIFFS.end();
  return true;
}

String ConfigController::getRawConfig()
{
  SPIFFS.begin();

  if (SPIFFS.exists(this->configFilePath))
  {
    File configFile = this->getFile("r");
    if (configFile && configFile.size())
    {
      String rawConfig;
      while (configFile.available())
      {
        rawConfig += char(configFile.read());
      }
      configFile.close();
      return rawConfig;
    }
  }

  SPIFFS.end();
}

bool ConfigController::writeRawConfig(String rawConfig)
{
  Serial.println("Trying to write the new conifguration...");
  Serial.println(rawConfig);

  if(!SPIFFS.begin()) {
    Serial.println("Error: Could not begin SPIFFS in ConfigController.cpp -> ConfigController:saveConfig()");
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
  SPIFFS.end();
  Serial.println("Finished writing the new configuration. The ESP8266 will reboot now...");
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
