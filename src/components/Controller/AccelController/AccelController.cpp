#include "AccelController.h"

AccelController::AccelController(ConfigController* configController, RadioDevice device)
  : AbstractController(configController, device)
{
  this->sensor = new IMU10DOF();
  this->sensor->setup();
  setPowerInterval = new Metro(3000);
}

bool AccelController::handleController()
{
  this->sensor->getAccel_Data();
  float xAccel = this->sensor->Axyz[0];
  xAccel = xAccel * 10;
  // speed range 0  - 100

  byte newSpeed = map(xAccel, -10, 10, 0, 100);
  //byte newSpeed = map(xAccel, -2.0, 2.0, 0, 100);
  //Serial.print("Sensor input = ");
  //Serial.print(xAccel);
  //Serial.print(" Sensor mapped input = ");
  //Serial.println(newSpeed);

  if (newSpeed > 100)
  {
    newSpeed = 100;
  }


  if (newSpeed >= 48 && newSpeed <= 53)
  {
    // nothing
  }
  else
  {
    targetSpeed = newSpeed;
  }


  if (targetSpeed >= previousSpeed)
  {
     previousSpeed = previousSpeed + changeRate;
  }
  if (targetSpeed <= previousSpeed)
  {
    previousSpeed = previousSpeed - changeRate;
  }

  // Serial.print("Current Speed: ");
  // Serial.print(previousSpeed);
  // Serial.print(" TargetSpeed:  ");
  // Serial.println(targetSpeed);
  processInput(previousSpeed);

  return true;
}

bool AccelController::enable()
{
  return true;
}

void AccelController::read()
{
  return;
}


void AccelController::write()
{
  return;
}
