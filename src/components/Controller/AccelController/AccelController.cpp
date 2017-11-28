#include "AccelController.h"
#include "components/Utility/Log.h"



/**
  USE CASE THAT WE TARGET:
  1. Driver accelerates the bike manually by pedaling
  2. The acceleration is picked up and is maintained by PACER
  3. In case the driver accelerates more the bike will maintain quicker speed
  4. In case the driver brakes hard enough (TODO:: define hard enought) the motor will disengage
  5. The driver needs to accelerate again in order to re-engage motor.
*/


// Useful links:
// https://github.com/tekdemo/MiniPID/blob/master/examples/Basic/Basic.ino
// https://gist.github.com/bradley219/5373998
// http://mechatronicstutorials.blogspot.de/2014/07/balancing-of-ball-on-beam-using-arduino.html
// https://playground.arduino.cc/Code/PIDLibrary
// https://www.youtube.com/watch?v=fusr9eTceEo

AccelController::AccelController(AbstractDevice device)
  : AbstractController(device)
{
  Log::Logger()->write(Log::Level::DEBUG, "Started Construction of AccelController: ");

  this->sensor = new IMU10DOF();
  this->sensor->setup();
  this->readSensorTimer = new Metro(100);
  this->readSensorTimer->reset();
  this->PIDControl = new PID(&input, &output, &setpoint, 2, 5, 1, DIRECT);
  this->PIDControl = new PID(&input, &output, &setpoint, 2, 5, 1, DIRECT);
  this->PIDControl->SetMode(AUTOMATIC);
  this->PIDControl->SetOutputLimits(-50, 50);

  Log::Logger()->write(Log::Level::DEBUG, "Finished Construction of AccelController: ");
}

bool AccelController::handleController()
{
  double power = 50.0;

  this->sensor->getAccel_Data();

  this->acceleration = this->sensor->Axyz[0];

  // NOT WORKING AS EXPECTED YET. Weighted against the last few readings.
  // this->acceleration = this->acceleration * 0.99 + this->sensor->Axyz[0] * 0.1;
  this->input = map(acceleration * 100, -100 , 100, 0, 100);

  if (this->readSensorTimer->check() == 1) {
    this->setpoint = this->input;
  }

  this->PIDControl->Compute();

  power = power + output;

  Serial.print("SP: ");
  Serial.print(setpoint);
  Serial.print("  IN: ");
  Serial.print(input);
  Serial.print("  OU: ");
  Serial.print(output);
  Serial.print("  PW: ");
  Serial.print(power);
  Serial.println();
  delay(10);
  return true;
}


bool AccelController::enable()
{
  return true;
}

bool AccelController::disable()
{
  return true;
}
