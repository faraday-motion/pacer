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
  const char *configFilePath;
  const char *factoryConfigPath;
  File getFile(const char *permission);
public:
  Config* config;
  String configString;

  ConfigController();
  bool loadConfig();
  bool saveConfig();
  bool setConfigString(String newConfigString);
  bool unsetConfigString();

  // Debug
  bool printConfig(JsonObject& json);

};


#endif
