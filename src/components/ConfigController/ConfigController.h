#ifndef ConfigController_h
#define ConfigController_h

#include <Arduino.h>
#include <ArduinoJson.h>

#include "FS.h"
#include "Config.h"
#include "ArduinoJson.h"


class ConfigController
{
private:
  String configFilePath;
  String factoryConfigPath;
  File getFile(const char *permission);
public:
  Config* config;

  ConfigController();
  bool loadConfig();
  bool setConfigString(String newConfigString);
  bool unsetConfigString();

  String getRawConfig();
  bool writeRawConfig(String rawConfig);
  
  // Debug
  bool printConfig(JsonObject& json);

};


#endif
