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
  int _HANDLE_CLIENT_INTERVAL = 500;
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
