#ifndef SPIFFS_CONFIG_H
#define SPIFFS_CONFIG_H
#include <vector>
#include <Arduino.h>
#include <ArduinoJson.h>
#include "FS.h"
#include "SPIFFS.h"
#include "../configuration/base/configbase.h"

class Spiffs_config
{
    private:
      fs::FS &fs = SPIFFS;
      bool deleteFile(String path);
    public:
      Spiffs_config(){
      };
       void loadBase(String path, byte &id, int &configuration);
       void load(Configbase *config);
       bool save(Configbase *config);
       bool remove(byte id);
       void removeAll();
       void load(String path, byte &id, int &configuration);
       std::vector<String> list();
};

#endif
