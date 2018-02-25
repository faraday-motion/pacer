#ifndef ICONFIGURABLE_H
#define ICONFIGURABLE_H
#include <ArduinoJson.h>

class IConfigurable {
   public:
     void getConfiguration(JsonObject &root);
     void setConfiguration(JsonObject &root);
};
#endif
