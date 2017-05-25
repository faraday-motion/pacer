#ifndef ConnectionManager_h
#define ConnectionManager_h

#include "Radio.h"
#include "Wifi.h"
#include "components/ConfigController/ConfigController.h"

class ConnectionManager
{
  ConfigController* configController;

public:

  ConnectionManager(ConfigController* configController);
  void setup();

  Radio* radio;
  Wifi*  wifi;

  void handleClientConnections();
  // TODO:: Implement this.
  // Radio* getRadio();
  // Wifi* getWifi();

};

#endif
