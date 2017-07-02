#ifndef AccelContrller_H
#define AccelContrller_H

#include "../AbstractController.h"
#include "components/Sensors/IMU10DOF/IMU10DOF.h"
#include <Metro.h>

class AccelController : public AbstractController
{
private:
  Metro* assistingTimer;
  IMU10DOF* sensor;

  byte previousSpeed = 50;
  byte targetSpeed   = 50;
  byte changeRate    = 10;
  byte newSpeed      = 50;
  float sampleSum    = 0;
  bool assisting     = false;
  byte lockedTarget  = 0;

public:

  AccelController(ConfigController* configController, RadioDevice device);

  void read();
  void write();
  bool enable();
  bool handleController();

};

#endif
