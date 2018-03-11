#ifndef ICONFIG_H
#define ICONFIG_H
#include <ArduinoJson.h>

class IConfig {
   public:
     virtual byte getId() = 0;
     virtual void getConfiguration(JsonObject &root) = 0;
     virtual void setConfiguration(JsonObject &root) = 0;
};
#endif
