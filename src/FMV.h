#ifndef FMV_h
#define FMV_h
#include "components/Controller/ControllerManager.h"
#include "components/Connection/ConnectionManager.h"
#include "components/ConfigController/ConfigController.h"
#include "components/Console/Console.h"
#include <Metro.h>

class Console; // Forward declaration to make the compiler happy.
class FMV {

  /* Metro timers */
  Metro* handleClientInterval;

  bool wasConfigured = 0;

public:
  ConnectionManager* connectionManager;
  ControllerManager* controllerManager;
  ConfigController* configController;
  const String version = "0.1";

  FMV();
  void loop();
  void setup();

  // Tasks?
  void handlePendingConnectionDevices();
  void registerControllers();

};


#endif
