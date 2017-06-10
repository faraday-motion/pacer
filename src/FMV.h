#ifndef FMV_h
#define FMV_h
#include "components/Communication/Console.h"
#include "components/ConfigController/ConfigController.h"
#include "components/Controller/ControllerManager.h"
#include "components/Connection/ConnectionManager.h"
#include <Metro.h>

class FMV {
  Console* console;
  ConfigController* configController;
  ConnectionManager* connectionManager;
  ControllerManager* controllerManager;

  Metro* printRegisteredControllers;

public:
  FMV();
  void loop();
  void setup();

  // Tasks?
  void registerPendingConnectionDevices();

};


#endif
