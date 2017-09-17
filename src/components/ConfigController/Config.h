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
  _webscocket             websocket;
  _modules                modules;
  byte                    motorCount;

  byte                    authorizedControllersCount;
  _authorizedControllers  authorizedControllers[5];

  byte                    registeredControllersCount;
  _registeredController   registeredControllers[5];

  Config();
  void setConfig(JsonObject& json);

  // Configurators
  void configureRegisteredControllers(JsonObject& json);
  void configureWifi(JsonObject& json);
  void configureWebSockets(JsonObject& json);
  void configureVehicle(JsonObject& json);
  void configureModules(JsonObject& json);

  // Debug
  void printConfig();
  void printWifi();
  void printRegisteredControllers();
};


#endif
