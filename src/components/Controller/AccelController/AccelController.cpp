#include "AccelController.h"

AccelController::AccelController(ConfigController* configController, RadioDevice device)
  : AbstractController(configController, device)
{
  this->sensor = new IMU10DOF();
  this->sensor->setup();
  this->assistingTimer = new Metro(3000);
  this->assistingTimer->reset(); // Required. Otherwise the timer is set in the future.
}

bool AccelController::handleController()
{
  this->sensor->getAccel_Data();
  float xAccel = this->sensor->Axyz[0];

  float samples[5];

  // For 50ms we are gathering samples.
  for (size_t i = 0; i < 5; i++) {
    this->sensor->getAccel_Data();
    samples[i] = this->sensor->Axyz[0];
    delay(5); //NOTE:: Delays are really not good.
  }

  // Get the average X axeleration

  float sum = 0;
  for (size_t s = 0; s < 5; s++) {
    sum = sum + samples[s];
    //Serial.print(samples[s]);
    //Serial.print(", ");
  }


  float average = sum / 5; // average of 10 samples.

  if (average < 0.05 && average > -0.05 )
  {
     // We are not moving anywhere;
     average = 0;
  }
  else if (average > 1)
  {
    average = 1;
  } else if (average < -1)

  {
    average = -1;
  }

  // For the sake of the range:
  average = average * 100;
  newSpeed = map(average, -40, 40, 20, 100);

  if (newSpeed < 0)
  {
    newSpeed = 0;
  }
  if (newSpeed > 100)
  {
    newSpeed = 100;
  }

  if (newSpeed > 55)
  {
    Serial.println(":::::::TICK ACCCEL::::::::::");
    this->lockedTarget = newSpeed;
    this->assisting    = true;
  }
  else if (newSpeed < 45)
  {
    Serial.println(":::::::TICK BREAK::::::::::");
    this->lockedTarget = newSpeed;
  } else {
     if (this->assisting == true)
     {
       newSpeed = this->lockedTarget;
     }
  }

  if (lockedTarget >= previousSpeed)
  {
     // Accelerating
     previousSpeed = previousSpeed + changeRate;
  }

  if (lockedTarget <= previousSpeed)
  {
    // Braking
    previousSpeed = previousSpeed - changeRate;
  }


  //if (this->assisting == true)
  //{


  Serial.print("previousSpeed ");
  Serial.println(previousSpeed);
  processInput(previousSpeed);
  //}



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
