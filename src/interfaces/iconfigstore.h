#ifndef ICONFIGSTORE_H
#define ICONFIGSTORE_H

#include <vector>
#include <Arduino.h>
#include "./iconfig.h"

class IConfigStore {
   public:
     virtual bool load(String path, byte &id, int &configuration) = 0;
     virtual bool load(IConfig * config) = 0;
     virtual bool save(IConfig * config) = 0;
     virtual bool remove(byte id) = 0;
     virtual bool remove() = 0;
     virtual bool list(std::vector<String> &files) = 0;
};
#endif
