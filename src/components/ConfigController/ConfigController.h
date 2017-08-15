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
  File getFile(const char *permission, bool factoryConfig = false);
  bool beginSPIFFS();
  bool endSPIFFS();
public:
  Config* config;

  ConfigController();
  bool hasLoadedConfig = false;
  bool loadConfig();
  bool setConfigString(String newConfigString);
  bool unsetConfigString();

  String getRawConfig(bool factoryConfig = false);
  bool writeRawConfig(String rawConfig);

  bool restoreFactoryConfig();

  // Debug
  bool printConfig(JsonObject& json);

};


#endif
