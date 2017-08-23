#include "CurrentController.h"
#include "components/Utility/Log.h"

CurrentController::CurrentController()
{
}


void CurrentController::setup(Config* configuration)
{
  Log::Logger()->write(Log::Level::DEBUG, "Setting up the CurrentController...");
  config = configuration;

  //Current control
  defaultCurrentNeutral         = config->currentControl.defaultCurrentNeutral;
  defaultCurrentAccelerationMax = config->currentControl.defaultCurrentAccelerationMax;
  defaultCurrentAccelerationMin = config->currentControl.defaultCurrentAccelerationMin;
  defaultCurrentBrakeMax        = config->currentControl.defaultCurrentBrakeMax;
  defaultCurrentBrakeMin        = config->currentControl.defaultCurrentBrakeMin;

  // Seting the default values and states for the controller;
  defaultInputNeutral           = config->controller.defaultInputNeutral;
  defaultInputMinBrake          = config->controller.defaultInputMinBrake;
  defaultInputMaxBrake          = config->controller.defaultInputMaxBrake;
  defaultInputMinAcceleration   = config->controller.defaultInputMinAcceleration;
  defaultInputMaxAcceleration   = config->controller.defaultInputMaxAcceleration;
  defaultSmoothAlpha            = config->controller.defaultSmoothAlpha;
  Log::Logger()->write(Log::Level::DEBUG, "Finished setting up the CurrentController...");

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
