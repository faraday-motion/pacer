#include "Controller.h"

#include "../ConfigController/Config.h"
#include "../ConfigController/ConfigController.h"

Controller::Controller(ConfigController* configController)
{
  config = configController->config;
  // Pass the config pointer to the current controller.

}

void Controller::setup(MotorController* motorController)
{
  // Setting the pointers to Motor and Wifi.
  //this->motorController = motorController;
  Serial.println("Controller is setting up the CurrentController");
  currentController.setup(config);
  // Seting the default values and states for the controller;
  defaultInputNeutral         = config->controller.defaultInputNeutral;
  defaultInputMinBrake        = config->controller.defaultInputMinBrake;
  defaultInputMaxBrake        = config->controller.defaultInputMaxBrake;
  defaultInputMinAcceleration = config->controller.defaultInputMinAcceleration;
  defaultInputMaxAcceleration = config->controller.defaultInputMaxAcceleration;
  defaultSmoothAlpha          = config->controller.defaultSmoothAlpha;



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

  // here I think we need to set the current input measurements.
  if (previousInput > defaultInputMinBrake && previousInput < defaultInputMinAcceleration)
  {
    // we are in neutral.
    float motorCurrent = currentController.getNeutralCurrent();
    motorController->set_current(motorCurrent);

    // Serial.print("Neutral ::: Current = ");
    // Serial.println(motorCurrent);
    return 0;
  }

  if (previousInput >= defaultInputMinAcceleration)
  {
    //accelerating
    float motorCurrent = currentController.getMotorAccelerationCurrent(previousInput);
    motorController->set_current(motorCurrent);

    // Serial.print("Acceller ::: Current = ");
    // Serial.println(motorCurrent);
    return 1;
  }

  // braking;
  float motorCurrent = currentController.getMotorBrakingCurrent(previousInput);
  motorController->set_current_brake(motorCurrent);

  // Serial.print("Braking ::: Current = ");
  // Serial.println(motorCurrent);
  return 1;

}

void Controller::processInput(byte latestInput)
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
