#ifndef FMV_h
#define FMV_h
#include "components/Communication/Console.h"
#include "components/ConfigController/ConfigController.h"
#include "components/Controller/ControllerManager.h"
#include "components/Connection/ConnectionManager.h"
#include "components/Sensors/IMU10DOF/IMU10DOF.h"  // Required for Wire.
#include <Metro.h>

class FMV {

  /* Metro timers */
  Metro* handleClientInterval;

  Console* console;
  ConfigController* configController;
  ConnectionManager* connectionManager;
  ControllerManager* controllerManager;

public:
  bool stopScanning = false;
  FMV();
  void loop();
  void setup();

  // Tasks?
  void handlePendingConnectionDevices();

};


#endif
