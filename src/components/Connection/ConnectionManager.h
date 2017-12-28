#ifndef ConnectionManager_h
#define ConnectionManager_h

#include <Metro.h>
#include "Wifi/Wifi.h"
#include "Radio/Radio.h"
#include "Radio/RadioHandshake.h"
#include "WebServer/WebServer.h"
#include "WebSocket/WebSocketCommunicator.h"
#include "components/Controller/ControllerManager.h"
#include "components/Device/AbstractDevice.h"

class ControllerManager;
class RadioHandshake;

class ConnectionManager
{
private:
  // Must be smaller then the _LOST_CONNECTION interval in specific  connections classes.
  int _HANDLE_CLIENT_INTERVAL = 150;
  // Object Pointers
  Metro* handleClientInterval;
  ControllerManager* controllerManager;

  void handleWifiConnections();
  void handleRadioConnections();
  void healthCheckWebSocketConnections();

public:

  ConnectionManager();
  void setup();
  void loop();
  void bindControllerManager(ControllerManager* ControllerManager);

  Radio* radio;
  RadioHandshake* radioHandshake;
  Wifi*  wifi;
  WebSocketCommunicator* ws;
  WebServer* webServer;

  AbstractDevice pendingDevice;

  void handleClientConnections();
  void clearPendingDevice();

};

#endif
