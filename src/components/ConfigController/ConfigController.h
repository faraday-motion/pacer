#ifndef ConfigController_h
#define ConfigController_h

#include <ArduinoJson.h>
#include "FS.h"

class ConfigController
{
  const char *configFilePath;
public:
  ConfigController(); 
  bool getConfig();
  bool saveConfig();

  File getFile(const char *permission);
};


#endif
