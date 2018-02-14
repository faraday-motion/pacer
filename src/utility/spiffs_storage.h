#ifndef SPIFFS_STORAGE_H
#define SPIFFS_STORAGE_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include "FS.h"
#include "SPIFFS.h"
#include "../configuration/base/configbase.h"
#include "../interfaces/isend.h"

class Spiffs_storage
{
    private:
      fs::FS &fs = SPIFFS;
    public:
      Spiffs_storage(){
      };
      void append(String path, const String message);
      void read(String path, ISend* sender);
      bool remove(String path);
};

#endif
