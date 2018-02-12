#ifndef TOOLS_H
#define TOOLS_H
#include <vector>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <rom/rtc.h>
#include "FS.h"
#include "SPIFFS.h"
#include "../configuration/base/configbase.h"

class Tools
{
    private:
      Tools(){
      };
    public:
      static int analogReadMultiple(byte pin, byte readings);
      static bool digitalReadMultiple(byte pin, byte readings);
      static int getPosition(const char *array, size_t size, char c);
      static void byteToChar(byte* bytes, char* chars, unsigned int count);
      static void charToByte(char* chars, byte* bytes, unsigned int count);
      static String prefixInt(byte id, byte totalDigits = 3);
      static String dateTimeString(struct tm * timeinfo, const char * format);
      static byte getChecksum(byte* array, byte arraySize);
      static bool validateChecksum(byte* array, byte arraySize);
      static float mapFloat(float x, float in_min, float in_max, float out_min, float out_max);
      static String ipAddressToString(IPAddress ip);
      static void copyArray(byte* src, byte* dest, byte arraySize);
      static String resetReason(RESET_REASON reason);
};

#endif
