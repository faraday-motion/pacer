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
AccelController::AccelController(AbstractDevice device)
  : AbstractController(device)
{
  Log::Logger()->write(Log::Level::DEBUG, "Started Construction of AccelController: ");
  this->sensor = new IMU10DOF();
  this->sensor->setup();
  this->assistingTimer = new Metro(500);
  this->logTimer = new Metro(500);
  this->logTimer->reset();
  this->assistingTimer->reset(); // Required. Otherwise the timer is set in the future.
  Log::Logger()->write(Log::Level::DEBUG, "Finished Construction of AccelController: ");
}

bool AccelController::handleController()
{
  float samples[25];

  // Gathering samples.
  for (size_t i = 0; i < 25; i++) {
    this->sensor->getAccel_Data();
    samples[i] = this->sensor->Axyz[0];
  }

  // Get the average X axeleration
  float sum = 0;
  for (size_t s = 0; s < 25; s++) {
    sum = sum + samples[s];
  }
  float average = sum / 25; // average of 5 samples.

  // For the sake of the range:
  average  = average * 100;

  newSpeed = map(average, -100 , 100, 0, 100);
    delay(5);
    this->motorController->get_values(); // get the data from the motorController.
    delay(5);
    if (VescParams::Instance()->motorValues.rpm < 100)
    {
      newSpeed = 49;
    }
  // setting the boundaries for cases when the average is going out the min max.
  if (newSpeed < 0)
  {
    newSpeed = 0;
  }
  if (newSpeed > 100)
  {
    newSpeed = 100;
  }

  if (newSpeed > 50)
  {
    this->lockedTarget = newSpeed;
    this->assisting    = true;
  }

  if (newSpeed < 50)
  {
    newSpeed = 50;
    lockedTarget = 0;
    this->assisting = false;
  }

  delay(5);
  if (this->assisting == true)
  {
    // Only increase if we did not get to the max already
    if (previousSpeed < 100)
    {
      // Accelerating
      previousSpeed = previousSpeed + changeRate;
    } else {
      previousSpeed = 100;
    }

    delay(5);
    processInput(previousSpeed);

  } else {
    processInput(50);
  }

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
