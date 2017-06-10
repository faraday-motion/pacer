#ifndef ConnectionManager_h
#define ConnectionManager_h

#include "Radio.h"
#include "Wifi.h"
#include "components/ConfigController/ConfigController.h"
#include <Metro.h>

struct pendingDevice {
  byte id[5];
  byte type; // 0 - uknown, 1 - wifi, 2 - nunchuck.
  bool isWaiting = false;
};


class ConnectionManager
{
  ConfigController* configController;
  byte _HANDLE_CLIENT_INTERVAL = 250;
public:
  pendingDevice pendingDevices[2];
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
