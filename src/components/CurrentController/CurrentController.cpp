#include "CurrentController.h"
CurrentController::CurrentController()
{
  //Current control
  defaultCurrentNeutral         = 0;
  defaultCurrentAccelerationMax = 60;
  defaultCurrentAccelerationMin = 0.25;
  defaultCurrentBrakeMax        = 60;
  defaultCurrentBrakeMin        = 0;

  //TODO:: this needs to be coming out of a config file.
  // Seting the default values and states for the controller;
  defaultInputNeutral         = 50;
  defaultInputMinBrake        = 48;
  defaultInputMaxBrake        = 0;
  defaultInputMinAcceleration = 52;
  defaultInputMaxAcceleration = 100;
  defaultSmoothAlpha          = 0.5;

}

// Returns the amps required to be passed to the motorcontroller.
float CurrentController::getMotorAccelerationCurrent(float previousControllerInput)
{
  float motorCurrent = map(previousControllerInput,  defaultInputMinAcceleration, defaultInputMaxAcceleration, defaultCurrentNeutral, defaultCurrentAccelerationMax);
  //Hack function for making things more smooth
  float adjustedCurrent = ((motorCurrent * motorCurrent) / defaultCurrentAccelerationMax) + defaultCurrentAccelerationMin;
  adjustedCurrent = constrain(adjustedCurrent, defaultCurrentAccelerationMin, defaultCurrentAccelerationMax);

  return adjustedCurrent;
}

float CurrentController::getMotorBrakingCurrent(float previousControllerInput)
{
  float motorCurrent = map(previousControllerInput, defaultInputMinBrake, defaultInputMaxBrake, defaultCurrentBrakeMin, defaultCurrentBrakeMax);
  float adjustedCurrent = motorCurrent;

  adjustedCurrent = ((motorCurrent * motorCurrent) / defaultCurrentBrakeMax) + defaultCurrentBrakeMin;
  adjustedCurrent = constrain(adjustedCurrent, defaultCurrentBrakeMin, defaultCurrentBrakeMax);

  return adjustedCurrent;
}

float CurrentController::getNeutralCurrent()
{
  return defaultCurrentNeutral;
}
