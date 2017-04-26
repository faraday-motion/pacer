#include "Motor.h"

/**
  * Constructor
  */
Motor::Motor(MotorController* motorController){
  // Assign pointer to the motorController.
  this->motorController = motorController;

  // TODO:: this should be part of the BaseController
  // power
  defaultInputNeutral         = 50;
  defaultInputMinBrake        = 48;
  defaultInputMaxBrake        = 0;
  defaultInputMinAcceleration = 52;
  defaultInputMaxAcceleration = 100;
  defaultSmoothAlpha          = 0.5;

  //Motor
  motorDirection     = 0; //0 = Neutral, 1= Acc, 2= Brake, 3 = Reverse
  motorTargetPercent = 0;
  motorPercent       = 0;

  //Current control
  defaultCurrentNeutral         = 0;
  defaultCurrentAccelerationMax = 60;
  defaultCurrentAccelerationMin = 0.25;
  defaultCurrentBrakeMax        = 60;
  defaultCurrentBrakeMin        = 0;

  // Setting defaults control values
  controlPower  = defaultInputNeutral;
  controlTarget = defaultInputNeutral;
}

void Motor::setPower(byte target, byte controllerType)
{
    Motor::adjustPower(constrain(target, defaultInputMaxBrake, defaultInputMaxAcceleration));
    // Once the values are adjusted convert the Power.
    Motor::convertPower();
} // end setPower();


void Motor::adjustPower(byte target)
{

  // Smooth the input
  float targetAlpha = (defaultSmoothAlpha * target) + ((1 - defaultSmoothAlpha * controlPower));
  // If the value is close to target, set it to target
  if (abs(float(target) - float(targetAlpha)))
  {
    targetAlpha = target;
  }
  target = targetAlpha;



  if (target > defaultInputMinBrake && target < defaultInputMinAcceleration)
  {
    // When within the inner boundary, set the default neutral value
    target = defaultInputNeutral;
  }

  // THIS NEEDS TO BE PORTED TO THE CONTROLLER BASE CLASS.
    // if (FMV.controlCruiseControl == true && FMV.controlPower >= defaultInputMinAcceleration)
    // {
    //   // Set input to current output when on cruisecontrol and we are accelerating
    //   // Not set the controlpower and ignore the power input, maybe needs changing when using nunchuck/reading values from motorcontroller
    // }
    // else
    // {
    controlPower = constrain(target, defaultInputMaxBrake, defaultInputMaxAcceleration);
  //}
}


/**
  * Converts the outCurrentPower to the needed servopower
  */

void Motor::convertPower()
{
  if (controlPower > defaultInputMinBrake && controlPower < defaultInputMinAcceleration )
  {
    // Neutral
    motorDirection = 0;
    motorPercent = 0;
    motorTargetPercent = 0;

    motorController->set_current_brake(0);
  }
  else if (controlPower >= defaultInputMinAcceleration)
  {
    // Accelerating
    motorDirection = 1;
    motorTargetPercent = map(controlTarget, defaultInputMinBrake, defaultInputMaxAcceleration, 0, 100);
    // current percent of the power based sent from input from the controller.
    motorPercent = map(controlPower, defaultInputMinBrake, defaultInputMaxAcceleration, 0, 100);

    float motorCurrent = map(controlPower, defaultInputMinAcceleration, defaultInputMaxAcceleration, defaultCurrentNeutral, defaultCurrentAccelerationMax);
    //Hack function for making things more smooth
    float adjustedCurrent = ((motorCurrent * motorCurrent) / defaultCurrentAccelerationMax) + defaultCurrentAccelerationMin;
    motorController->set_current(adjustedCurrent);
  }
  else
  {
    // Breaking
    motorDirection = 2;
    motorTargetPercent = map(controlTarget, defaultInputMinBrake, defaultInputMaxBrake, 0, 100);


    motorPercent = map(controlPower, defaultInputMinBrake, defaultInputMaxBrake, 0, 100);


    float motorCurrent = map(controlPower, defaultInputMinBrake, defaultInputMaxBrake, defaultCurrentBrakeMin, defaultCurrentBrakeMax);
    float adjustedCurrent = motorCurrent;

    adjustedCurrent = ((motorCurrent * motorCurrent) / defaultCurrentBrakeMax) + defaultCurrentBrakeMin;
    adjustedCurrent = constrain(adjustedCurrent, defaultCurrentBrakeMin, defaultCurrentBrakeMax);

    motorController->set_current_brake(adjustedCurrent);

  } // end if accelerate, neutral or brake
} // end Motor::convertPower();


void Motor::setDefaultPower()
{
  // TODO:: This needs to be ported to Controller.h

  controlTarget = defaultInputNeutral;
  controlPower = defaultInputNeutral;

  // Motor
  motorDirection = 0;
  motorPercent = 0;
  motorTargetPercent = 0;

  motorController->set_current(defaultCurrentNeutral);
}


// THIS NEEDS TO BE PORTED TO Controller.h
// void Motor::setDeadSwitch()
// {
//   //TODO:: fix the often instantiation
//   FaradayMotionVehicle FMV;
//   FMV.controlDead = true;
// }
