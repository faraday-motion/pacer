#include <vector>
#include <algorithm>
#include <iterator>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <rom/rtc.h>
#include "SPIFFS.h"
#include "./tools.h"
#include "../configuration/default/configuration.h"
#include "../configuration/base/configbase.h"
#include "../configuration/configurator.h"
#include "../logs/logger.h"

int Tools::analogReadMultiple(byte pin, byte readings = 3)
{
  int sum = 0;
  for (int i=0; i<readings; i++)
  {
    sum += analogRead(pin);
    delayMicroseconds(1);
  }
  return sum/int(readings);
}

bool Tools::digitalReadMultiple(byte pin, byte readings = 3)
{
  int sum = 0;
  for (int i=0; i<readings; i++)
  {
    sum += (int)digitalRead(pin);
    delayMicroseconds(1);
  }
  if (sum == 0)
    return false;
  return true;
}

int Tools::getPosition(const char *array, size_t size, char c)
{
    const char* end = array + size;
    const char* match = std::find(array, end, c);
    return (end == match)? -1 : (match-array);
}

void Tools::charToByte(char* chars, byte* bytes, unsigned int count){
    for(unsigned int i = 0; i < count; i++)
        bytes[i] = (byte)chars[i];
}

void Tools::byteToChar(byte* bytes, char* chars, unsigned int count){
    for(unsigned int i = 0; i < count; i++)
         chars[i] = (char)bytes[i];
}

float Tools::mapFloat(float x, float in_min, float in_max, float out_min, float out_max)
{
 return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

String Tools::prefixInt(byte id, byte totalDigits)
{
  String s = String(id);
  while (s.length() < totalDigits)
    s = "0" + s;
  return s;
}

String Tools::dateTimeString(struct tm * timeinfo, const char * format)
{
    const char * f = format;
    if(!f){
        f = "%c";
    }
    char buf[64];
    size_t written = strftime(buf, 64, f, timeinfo);
    //print(buf);
    return buf;
}

byte Tools::getChecksum(byte* array, byte arraySize)
{
  long validateSum = 0;
  for (byte i = 0; i < arraySize; i++) {
    validateSum += array[i];
  }
  validateSum -= array[arraySize - 1];
  return validateSum % 256;
}

bool Tools::validateChecksum(byte* array, byte arraySize)
{
  return array[arraySize - 1] == getChecksum(array, arraySize);
}

String Tools::ipAddressToString(IPAddress ip)
{
  return (String)ip[0] + "." +
         (String)ip[1] + "." +
         (String)ip[2] + "." +
         (String)ip[3] + "." +
         (String)ip[4];
}

void Tools::copyArray(byte* src, byte* dest, byte arraySize)
{
  for(byte i=0; i<arraySize; i++){
    dest[i] = src[i];
  }
}

String Tools::resetReason(RESET_REASON reason)
{
  switch ( reason)
  {
    case 1  : return FPSTR("Vbat power on reset");break;
    case 3  : return FPSTR("Software reset digital core");break;
    case 4  : return FPSTR("Legacy watch dog reset digital core");break;
    case 5  : return FPSTR("Deep Sleep reset digital core");break;
    case 6  : return FPSTR("Reset by SLC module, reset digital core");break;
    case 7  : return FPSTR("Timer Group0 Watch dog reset digital core");break;
    case 8  : return FPSTR("Timer Group1 Watch dog reset digital core");break;
    case 9  : return FPSTR("RTC Watch dog Reset digital core");break;
    case 10 : return FPSTR("Instrusion tested to reset CPU");break;
    case 11 : return FPSTR("Time Group reset CPU");break;
    case 12 : return FPSTR("Software reset CPU");break;
    case 13 : return FPSTR("RTC Watch dog Reset CPU");break;
    case 14 : return FPSTR("for APP CPU, reseted by PRO CPU");break;
    case 15 : return FPSTR("Reset when the vdd voltage is not stable");break;
    case 16 : return FPSTR("RTC Watch dog reset digital core and rtc module");break;
    default : return FPSTR("NO_MEAN");
  }
}
