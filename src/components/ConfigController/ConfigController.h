#ifndef ConfigController_h
#define ConfigController_h

#include <ArduinoJson.h>
#include "FS.h"
#include "Config.h"

class ConfigController
{
private:
  File getFile(const char *permission);
  const char *configFilePath;
  const char *factoryConfigPath;
public:
  Config* config;
  String configString;

  ConfigController(Config* config);
  bool loadConfig();
  bool getJsonConfig();
  bool saveConfig();
  bool setConfigString(String newConfigString);
  bool unsetConfigString();

};


#endif
