#ifndef FMV_h
#define FMV_h
#include "components/Communication/Console.h"
#include "components/Controller/ControllerManager.h"
#include "components/Connection/ConnectionManager.h"
#include "components/ConfigController/ConfigController.h"

#include <Metro.h>

class FMV {

  /* Metro timers */
  Metro* handleClientInterval;

  Console* console;
  ConfigController* configController;
  ConnectionManager* connectionManager;
  ControllerManager* controllerManager;

  bool wasConfigured = 0; 

public:
  bool stopScanning = false;
  FMV();
  void loop();
  void setup();

  // Tasks?
  void handlePendingConnectionDevices();
  void registerWiredDevices();

};


#endif
