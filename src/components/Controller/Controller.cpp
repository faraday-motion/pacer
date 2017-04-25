

#include "Controller.h"

Controller::Controller()
{


}

void Controller::setup(Motor* motor, Wifi* wifi)
{
  // Setting the pointers to Motor and Wifi.
  this->motor = motor;
  this->wifi  = wifi;

  // Seting the default values and states for the controller;

  // Controller States
  controlDead          = false;
  controlEnabled       = false;
  controlCruiseControl = false;

  //Controller Values
  controllerType       = 0; //0= Nothing, 1= WiFi, 2=Nunchuk
}

// Checks the controller and enables the motor.
void Controller::setMotorPower(byte target)
{
  Serial.println("Setting motor power");
  // Verify if there's a controller enabled. If not then setDefaultPower
  // for the motor.
  if (controllerType != 0 && controlDead != false){
    motor->setPower(target, controllerType);
  }
  else
  {
    motor->setDefaultPower();
  }
}

// Set default power to the motor

void Controller::setMotorDefaultPower()
{
  Serial.println("baseController setting default power.");
  motor->setDefaultPower();
}


// TODO:: Move Controller Properties to Private group
// set getter and setter functions
