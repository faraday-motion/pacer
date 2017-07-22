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
  byte changeRate    = 5;
  byte newSpeed      = 50;
  float sampleSum    = 0;
  float average      = 0;
  bool assisting     = false;
  byte lockedTarget  = 0;
  long motorRpm = 0;
  /* start not too sensitive and as we build up speed we make it more and more sensite */
  byte computeSpeed(float sensorReading);
  unsigned int getSensitivity();
public:

  AccelController(ConfigController* configController, RadioDevice device);

  bool enable();
  bool disable();
  bool handleController();

};

#endif
