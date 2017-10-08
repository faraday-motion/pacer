#ifndef Config_h
#define Config_h

#include "ConfigStructs.h"
#include "ArduinoJson.h"


// TODO:: Make so that only configController can set parameters.
class Config
{
public:
  // Get Instance
  static Config* get();

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

  void setConfig(JsonObject& json);

  // Debug
  void printConfig();
  void printWifi();
  void printRegisteredControllers();
  void printAuthorizedControllerTypes();
protected:
  Config() {}; // Constructor is protected
  Config(Config const&) {}; // Copy constructor is protected

private:
  Config* _config = nullptr;
  static Config* config;

  // Configurators
  void configureRegisteredControllers(JsonObject& json);
  void configureWifi(JsonObject& json);
  void configureWebSockets(JsonObject& json);
  void configureVehicle(JsonObject& json);
  void configureModules(JsonObject& json);
};


#endif
