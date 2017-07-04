#include "AccelController.h"
#include "components/Utility/Log.h"
AccelController::AccelController(ConfigController* configController, RadioDevice device)
  : AbstractController(configController, device)
{
  this->sensor = new IMU10DOF();
  this->sensor->setup();
  this->assistingTimer = new Metro(500);
  this->logTimer = new Metro(100);
  this->assistingTimer->reset(); // Required. Otherwise the timer is set in the future.


}

bool AccelController::handleController()
{
  this->sensor->getAccel_Data();
  float xAccel = this->sensor->Axyz[0];

  float samples[5];


  // For we are gathering samples.
  for (size_t i = 0; i < 5; i++) {
    this->sensor->getAccel_Data();
    samples[i] = this->sensor->Axyz[0];
  }
  // Get the average X axeleration

  float sum = 0;
  for (size_t s = 0; s < 5; s++) {
    sum = sum + samples[s];
    //Serial.print(samples[s]);
    //Serial.print(", ");
  }
  float average = sum / 5; // average of 10 samples.

  // if (average < 0.05 && average > -0.05 )
  // {
  //    // We are not moving anywhere;
  //    average = 0;
  // }
  // else if (average > 1)
  // {
  //   average = 1;
  // } else if (average < -1)
  //
  // {
  //   average = -1;
  // }

  // For the sake of the range:
  average  = average * 100;
  newSpeed = map(average, -100, 100, 0, 100);

  if (newSpeed < 0)
  {
    newSpeed = 0;
  }
  if (newSpeed > 100)
  {
    newSpeed = 100;
  }
  Serial.print("average:");
  Serial.print(average);
  Serial.print(" newSpeed: ");
  Serial.print(newSpeed);
  Serial.print(" lockedTarget: ");
  Serial.println(this->lockedTarget);

  if (this->assistingTimer->check() == 1) {
    Log::Instance()->logAccel(average, newSpeed, this->lockedTarget, previousSpeed);
  }


  if (newSpeed > 60)
  {
    Serial.println(":::::::Assist is ON! ACCCEL::::::::::");

    // Make sure we dont have a miss sync of the lockedTarget and the newSpeed
    //if (newSpeed >= previousSpeed)
    //{
      this->lockedTarget = newSpeed;
      //this->lockedTarget = 100;
      this->assisting    = true;
    //}

  }

  if (newSpeed < 40)
  {
    newSpeed = 50;
    lockedTarget = 0;
    this->assisting = false;
  }

  if (this->assisting == true)
  {
    previousSpeed = 100;
    // Serial.println("time to accelerate to target by 1");
    // if (lockedTarget >= previousSpeed)
    // {
    //    // Only increase if we did not get to the max already
    //    if (previousSpeed < 100)
    //    {
    //      // Accelerating
    //      previousSpeed = previousSpeed + changeRate;
    //    }
    //
    // }
    // else if (lockedTarget < previousSpeed)
    // {
    //   previousSpeed = 50;
    // }



    // DUDE! This automatically compensates for the acceleration force and brings the speed back to neutral. DUH!
    // if (lockedTarget <= previousSpeed)
    // {
    //   // Braking
    //   //previousSpeed = previousSpeed - changeRate;
    // }

      Serial.print("currentSpeed:  ");
      Serial.println(previousSpeed);
      processInput(previousSpeed);
  } else {
    processInput(50);
  }

  return true;

}








  // // speed range 0  - 100
  // if (xAccel > 0) {
  //   sampleSum = sampleSum + xAccel;
  // }
  //
  //
  // if (this->samplingInterval->check() == 1)
  // {
  //
  //   // Take a new conversion.
  //   // newSpeed = map(sampleSum, -10, 10, 0, 100);
  //   // Serial.print("sampleSume: ");
  //   // Serial.print(sampleSum);
  //   // Serial.print(" Converted newSpeed: ");
  //   // Serial.println(newSpeed);
  //   // sampleSum = 0;
  // }
  //
  //
  //
  //
  // //byte newSpeed = map(xAccel, -2.0, 2.0, 0, 100);
  // //Serial.print("Sensor input = ");
  // //Serial.print(xAccel);
  // //Serial.print(" Sensor mapped input = ");
  // //Serial.println(newSpeed);
  //
  // if (newSpeed > 100)
  // {
  //   newSpeed = 100;
  // }
  //
  //
  // // if (newSpeed >= 48 && newSpeed <= 53)
  // // {
  // //   // nothing
  // // }
  // // else
  // // {
  //   targetSpeed = newSpeed;
  // // }
  // // Serial.print("New Speed: ");
  // // Serial.print(newSpeed);
  // // Serial.print("Current Speed: ");
  // // Serial.print(previousSpeed);
  // // Serial.print(" TargetSpeed:  ");
  // // Serial.println(targetSpeed);
  // processInput(targetSpeed);
  //
  // if (targetSpeed >= previousSpeed)
  // {
  //    // Accelerating
  //    previousSpeed = previousSpeed + changeRate;
  // }
  // if (targetSpeed <= previousSpeed)
  // {
  //   // Braking
  //   previousSpeed = previousSpeed - changeRate;
  // }


//
//   return true;
// }

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
