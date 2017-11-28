#ifndef AccelContrller_H
#define AccelContrller_H

#include <Metro.h>
#include <PID_v1.h>
#include "../AbstractController.h"
#include "components/Sensors/IMU10DOF/IMU10DOF.h"
#include "components/MotorController/VescParams.h"




class AccelController : public AbstractController
{
private:
  Metro* readSensorTimer;
  Metro* logTimer;
  IMU10DOF* sensor;
  float motorRpm = 0.0;


  /* start not too sensitive and as we build up speed we make it more and more sensite */
  byte computeSpeed(float sensorReading);
public:

  AccelController(AbstractDevice device);

  PID* PIDControl;
  double input = 0;
  double setpoint = 50;
  double output = 0;
  double acceleration = 0;

  bool enable();
  bool disable();
  bool handleController();

};

#endif
