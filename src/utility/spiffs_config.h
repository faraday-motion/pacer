#ifndef SPIFFS_CONFIG_H
#define SPIFFS_CONFIG_H
#include <vector>
#include <Arduino.h>
#include <ArduinoJson.h>
#include "FS.h"
#include "SPIFFS.h"
#include "../configuration/base/configbase.h"
#include "../interfaces/interfaces.hpp"


class Spiffs_config : public virtual IConfigStore
{
    private:
      IStore * pStore;
    public:
      Spiffs_config(IStore * store) : IConfigStore() {
        pStore = store;
      };
      bool load(String path, byte &id, int &configuration);
      bool load(IConfig * config);
      bool save(IConfig * config);
      bool remove(byte id);
      bool remove();
      bool list(std::vector<String> &files);
};

#endif
