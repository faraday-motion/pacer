#include "Controller.h"
#include "../CurrentController/CurrentController.h"


Controller::Controller()
{

}

void Controller::setup(MotorController* motorController)
{
  // Setting the pointers to Motor and Wifi.
  this->motorController = motorController;

  // Seting the default values and states for the controller;
  defaultInputNeutral         = 50;
  defaultInputMinBrake        = 48;
  defaultInputMaxBrake        = 0;
  defaultInputMinAcceleration = 52;
  defaultInputMaxAcceleration = 100;
  defaultSmoothAlpha          = 0.5;

  // Setting inputs to neutral;
  latestInput                 = defaultInputNeutral;
  previousInput               = defaultInputNeutral;

  // Controller States
  controlDead          = false;
  controlEnabled       = false;
  controlCruiseControl = false;

  //Controller Values
  controllerType       = 0; //0= Nothing, 1= WiFi, 2=Nunchuk
}

// Decides the state of the motor
bool Controller::setMotorPower()
{
  CurrentController currentController;
  // here I think we need to set the current input measurements.
  if (previousInput > defaultInputMinBrake && previousInput < defaultInputMinAcceleration)
  {
    // we are in neutral.
    Serial.print("Neutral ::: Controller Input = ");
    Serial.println(previousInput);

    return 0;
  }

  if (previousInput >= defaultInputMinAcceleration)
  {
    //accelerating
    float motorCurrent = currentController.getMotorAccelerationCurrent(previousInput);

    Serial.print("Accelerating ::: Current = ");
    Serial.println(motorCurrent);
    return 0;
  }

  // braking;




  float motorCurrent = currentController.getMotorBrakingCurrent(previousInput);
  Serial.print("Braking ::: Current = ");
  Serial.println(motorCurrent);
  return 0;

}

void Controller::smoothenInput(byte latestInput)
{
  float targetAlpha = (defaultSmoothAlpha * latestInput) + ((1 - defaultSmoothAlpha * previousInput));

  // If the value is close to target, set it to target
  if(abs(float(latestInput) - float(targetAlpha)))
  {
    targetAlpha = latestInput;
  }
  if (latestInput > defaultInputMinBrake && latestInput < defaultInputMinAcceleration)
  {
    // When within the neutral boundary, set the default neutral value
    latestInput = defaultInputNeutral;
  }
  previousInput = constrain(latestInput, defaultInputMaxBrake, defaultInputMaxAcceleration);

  Controller::setMotorPower();
}



// TODO:: Move Controller Properties to Private group
// set getter and setter functions
