#ifndef ISTORE_H
#define ISTORE_H

#include <Arduino.h>
#include "SPIFFS.h"

class IStore {
   public:
     virtual bool save(String path, const String message) = 0;
     virtual bool append(String path, const String message, size_t maxSize) = 0;
     virtual bool read(String path, File &file) = 0;
     virtual bool remove(String path) = 0;
};
#endif
