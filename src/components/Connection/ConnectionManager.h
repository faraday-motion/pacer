#ifndef ConnectionManager_h
#define ConnectionManager_h

#include <Metro.h>
#include "Wifi/Wifi.h"
#include "Radio/Radio.h"
#include "WebServer/WebServer.h"
#include "WebSocket/WebSocketCommunicator.h"
#include "components/Device/AbstractDevice.h"

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
  WebSocketCommunicator* ws;
  WebServer* webServer;

  AbstractDevice pendingDevice;

  void handleClientConnections();
  void clearPendingDevice();

};

#endif
