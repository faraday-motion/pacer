#ifndef ConnectionManager_h
#define ConnectionManager_h

#include "Radio.h"
#include "Wifi.h"
#include "components/ConfigController/ConfigController.h"
#include <Metro.h>
#include "RadioDevice.h"


class ConnectionManager
{
  ConfigController* configController;
  byte _HANDLE_CLIENT_INTERVAL = 750;
public:
  RadioDevice pendingDevice;
  Metro* handleClientInterval;
  ConnectionManager(ConfigController* configController);
  void setup();

  Radio* radio;
  Wifi*  wifi;

  void handleClientConnections();
  // TODO:: Implement this.
  // Radio* getRadio();
  // Wifi* getWifi();

  void clearPendingDevices();

};

#endif
