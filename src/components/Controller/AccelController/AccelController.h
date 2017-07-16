#ifndef AccelContrller_H
#define AccelContrller_H

#include <Metro.h>
#include "../AbstractController.h"
#include "components/Sensors/IMU10DOF/IMU10DOF.h"
#include "components/MotorController/VescParams.h"



class AccelController : public AbstractController
{
private:
  Metro* assistingTimer;
  Metro* logTimer;
  IMU10DOF* sensor;

  byte previousSpeed = 50;
  byte targetSpeed   = 50;
  byte changeRate    = 1;
  byte newSpeed      = 50;
  float sampleSum    = 0;
  float average      = 0;
  bool assisting     = false;
  byte lockedTarget  = 0;
  long motorRpm = 0;
  /* start not too sensitive and as we build up speed we make it more and more sensite */
  byte computeSpeed(float sensorReading);

public:

  AccelController(ConfigController* configController, RadioDevice device);
  unsigned int getSensitivity();
  void read();
  void write();
  bool enable();
  bool handleController();

};

#endif
