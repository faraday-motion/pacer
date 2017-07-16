#ifndef ConnectionManager_h
#define ConnectionManager_h

#include "Wifi.h"
#include "Radio.h"
#include <Metro.h>
#include "RadioDevice.h"
#include "components/ConfigController/ConfigController.h"


class ConnectionManager
{
private:
  // Must be smaller then the _LOST_CONNECTION interval in specific  connections classes.
  int _HANDLE_CLIENT_INTERVAL = 150;

  // Object Pointers
  Metro* handleClientInterval;
  ConfigController* configController;

public:

  ConnectionManager(ConfigController* configController);
  void setup();

  Radio* radio;
  Wifi*  wifi;
  RadioDevice pendingDevice;

  void handleClientConnections();
  void clearPendingDevice();

};

#endif
