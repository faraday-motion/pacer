#ifndef AccelContrller_H
#define AccelContrller_H

#include "../AbstractController.h"
#include "components/Sensors/IMU10DOF/IMU10DOF.h"
#include <Metro.h>

class AccelController : public AbstractController
{
private:
  Metro* setPowerInterval;
  IMU10DOF* sensor;

  byte previousSpeed = 50;
  byte targetSpeed   = 50;
  byte changeRate    = 5;
public:

  AccelController(ConfigController* configController, RadioDevice device);

  void read();
  void write();
  bool enable();
  bool handleController();

};

#endif
