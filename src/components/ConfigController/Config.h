#ifndef Config_h
#define Config_h

#include "ConfigStructs.h"
#include "ArduinoJson.h"

class Config
{
public:
  _wifiConfig             wifi;
  _controllerConfig       controller;
  _currentControlConfig   currentControl;
  _wiredDevice            wiredDevices[5];
  _webscocket             websocket;
  byte                    wiredDevicesCount;
  byte                    motorCount;
  Config();
  void setConfig(JsonObject& json);


  // Configurators
  void configureWiredDevices(JsonObject& json);
  void configureWifi(JsonObject& json);
  void configureWebSockets(JsonObject& json);
  void configureVehicle(JsonObject& json);

  // Debug
  void printConfig();
  void printWifi();
  void printWiredDevices();
};


#endif
