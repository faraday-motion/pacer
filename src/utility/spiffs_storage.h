#ifndef SPIFFS_STORAGE_H
#define SPIFFS_STORAGE_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include "FS.h"
#include "SPIFFS.h"
#include "../configuration/base/configbase.h"
#include "../interfaces/interfaces.hpp"

class Spiffs_storage : public virtual IStore
{
    private:
      fs::FS &fs = SPIFFS;
    public:
      Spiffs_storage() : IStore(){
      };
      bool save(String path, const String message);
      bool append(String path, const String message, size_t maxSize = 0);
      bool read(String path, ISend * sender);
      bool read(String path, File &file);
      bool remove(String path);
};

#endif
