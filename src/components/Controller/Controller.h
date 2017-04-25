#ifndef Controller_h
#define Controller_h
#include "../Motor/Motor.h"
#include "../Connection/Wifi.h"
class Controller
{
public:
  // Motor
  Motor* motor;
  // Wifi
  Wifi* wifi;

  // Controller States
  bool controlDead;
  bool controlEnabled;
  bool controlCruiseControl;

  //Controller Values
  byte controllerType;

  //float controlPower; // moved to the motor. It seems the controller should not care what is the value of this variable.
  //int   controlTarget; // moved to the motor. It seems the controller should not care what is the value of this variable.

  Controller();
  void setup(Motor* motor, Wifi* wifi);
  // Sends the computed controller targe to the motor.
  void setMotorPower(byte target);
  void setMotorDefaultPower();
};

#endif
