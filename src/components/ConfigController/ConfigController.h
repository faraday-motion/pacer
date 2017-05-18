#ifndef ConfigController_h
#define ConfigController_h

#include <ArduinoJson.h>
#include "FS.h"
#include "Config.h"

class ConfigController
{
private:
  const char *configFilePath;
  const char *factoryConfigPath;
  File getFile(const char *permission);
  bool beginSPIFFS();
  bool endSPIFFS();
public:
  Config* config;
  String configString;

  ConfigController();
  bool loadConfig();
  bool getJsonConfig();
  bool saveConfig();
  bool setConfigString(String newConfigString);
  bool unsetConfigString();



};


#endif
