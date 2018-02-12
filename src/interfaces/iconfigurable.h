#ifndef ICONFIGURABLE_H
#define ICONFIGURABLE_H
#include <ArduinoJson.h>

class IConfigurable {
   public:
     virtual void getConfiguration(JsonObject &root) = 0;
     virtual void setConfiguration(JsonObject &root) = 0;
};
#endif
