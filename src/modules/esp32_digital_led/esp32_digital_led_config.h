#ifndef ESP32_DIGITAL_LED_CONFIG_H
#define ESP32_DIGITAL_LED_CONFIG_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../../configuration/default/configuration.h"
#include "../../configuration/default/default_pins.h"
#include "../../configuration/base/configbase.h"

class Esp32_digital_led_config : public Configbase{
public:
  Esp32_digital_led_config(byte id) : Configbase(id, Configurations::ESP32_DIGITAL_LED_CONFIG) {
  }

  void getModuleConfiguration(JsonObject &root)
  {
    root["pin"] = pin;
    root["pixelCount"] = pixelCount;
    root["brightness"] = brightness;
    root["interval"] = interval;
    root["critical"] = critical;
    root["backLedStartIndex"] = backLedStartIndex;
    root["backLedEndIndex"] = backLedEndIndex;
    root["leftLedStartIndex"] = leftLedStartIndex;
    root["leftLedEndIndex"] = leftLedEndIndex;
    root["frontLedStartIndex"] = frontLedStartIndex;
    root["frontLedEndIndex"] = frontLedEndIndex;
    root["rightLedStartIndex"] = rightLedStartIndex;
    root["rightLedEndIndex"] = rightLedEndIndex;
    root["dashboardLedStartIndex"] = dashboardLedStartIndex;
    root["dashboardLedEndIndex"] = dashboardLedEndIndex;
  }

  void setModuleConfiguration(JsonObject &root)
  {
    pin = byte(root["pin"]);
    pixelCount = byte(root["pixelCount"]);
    brightness = byte(root["brightness"]);
    interval = int(root["interval"]);
    critical = int(root["critical"]);
    backLedStartIndex = byte(root["backLedStartIndex"]);
    backLedEndIndex = byte(root["backLedEndIndex"]);
    leftLedStartIndex = byte(root["critical"]);
    leftLedEndIndex = byte(root["critical"]);
    frontLedStartIndex = byte(root["critical"]);
    frontLedEndIndex = byte(root["critical"]);
    rightLedStartIndex = byte(root["critical"]);
    rightLedEndIndex = byte(root["critical"]);
    dashboardLedStartIndex = byte(root["dashboardLedStartIndex"]);
    dashboardLedEndIndex = byte(root["dashboardLedEndIndex"]);
  }

  byte pin = PIN_NEOPIXELS;
  byte pixelCount = NEOPIXELS_PIXELS;
  byte brightness = 255;
  int interval = 50;
  int critical = 100;
  byte backLedStartIndex = 0;
  byte backLedEndIndex = 2;
  byte leftLedStartIndex = 3;
  byte leftLedEndIndex = 5;
  byte frontLedStartIndex = 6;
  byte frontLedEndIndex = 8;
  byte rightLedStartIndex = 9;
  byte rightLedEndIndex = 11;
  byte dashboardLedStartIndex = 0;
  byte dashboardLedEndIndex = 11;
};

#endif
