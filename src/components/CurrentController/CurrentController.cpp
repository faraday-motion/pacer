#include "CurrentController.h"
CurrentController::CurrentController()
{
}


void CurrentController::setup(Config* configuration)
{
  config = configuration;

  //Current control
  defaultCurrentNeutral         = config->currentControl.defaultCurrentNeutral;
  defaultCurrentAccelerationMax = config->currentControl.defaultCurrentAccelerationMax;
  defaultCurrentAccelerationMin = config->currentControl.defaultCurrentAccelerationMin;
  defaultCurrentBrakeMax        = config->currentControl.defaultCurrentBrakeMax;
  defaultCurrentBrakeMin        = config->currentControl.defaultCurrentBrakeMin;

  // Seting the default values and states for the controller;
  defaultInputNeutral         = config->controller.defaultInputNeutral;
  defaultInputMinBrake        = config->controller.defaultInputMinBrake;
  defaultInputMaxBrake        = config->controller.defaultInputMaxBrake;
  defaultInputMinAcceleration = config->controller.defaultInputMinAcceleration;
  defaultInputMaxAcceleration = config->controller.defaultInputMaxAcceleration;
  defaultSmoothAlpha          = config->controller.defaultSmoothAlpha;


  // Serial.println("The CurrrentControler Configured::::::::::::");
  // Serial.println(":::::: CURRENT VALUES  :::::::");
  // Serial.print("defaultCurrentNeutral :: ");
  // Serial.println(defaultCurrentNeutral);
  // Serial.print("defaultCurrentAccelerationMax :: ");
  // Serial.println(defaultCurrentAccelerationMax);
  // Serial.print("defaultCurrentAccelerationMin :: ");
  // Serial.println(defaultCurrentAccelerationMin);
  // Serial.print("defaultCurrentBrakeMax :: ");
  // Serial.println(defaultCurrentBrakeMax);
  // Serial.print("defaultCurrentBrakeMin :: ");
  // Serial.println(defaultCurrentBrakeMin);
  //
  // Serial.println(":::::: INPUT VALUES  :::::::");
  // Serial.print("defaultInputNeutral :: ");
  // Serial.println(defaultInputNeutral);
  // Serial.print("defaultInputMinBrake :: ");
  // Serial.println(defaultInputMinBrake);
  // Serial.print("defaultInputMaxBrake :: ");
  // Serial.println(defaultInputMaxBrake);
  // Serial.print("defaultInputMinAcceleration :: ");
  // Serial.println(defaultInputMinAcceleration);
  // Serial.print("defaultInputMaxAcceleration :: ");
  // Serial.println(defaultInputMaxAcceleration);
  // Serial.print("defaultSmoothAlpha :: ");
  // Serial.println(defaultSmoothAlpha);




}

// Returns the amps required to be passed to the motorcontroller.
float CurrentController::getMotorAccelerationCurrent(float previousControllerInput)
{
  float motorCurrent = map(previousControllerInput,  defaultInputMinAcceleration, defaultInputMaxAcceleration, defaultCurrentNeutral, defaultCurrentAccelerationMax);
  //Hack function for making things more smooth
  float adjustedCurrent = ((motorCurrent * motorCurrent) / defaultCurrentAccelerationMax) + defaultCurrentAccelerationMin;
  adjustedCurrent = constrain(adjustedCurrent, defaultCurrentAccelerationMin, defaultCurrentAccelerationMax);
  //Serial.println("Step 3");
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
