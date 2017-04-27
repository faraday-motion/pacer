#ifndef Controller_h
#define Controller_h
#include "../MotorController/MotorController.h"
#include "../Connection/Wifi.h"
class Controller
{
public:
  // Motor
  MotorController* motorController;

  // Controller Inputs
  byte  latestInput;
  float previousInput;

  // Controller States
  bool controlDead;
  bool controlEnabled;
  bool controlCruiseControl;

  //Controller Values
  byte controllerType;

  // Controller Constraints
  byte defaultInputNeutral;
  byte defaultInputMinBrake;
  byte defaultInputMaxBrake;
  byte defaultInputMinAcceleration;
  byte defaultInputMaxAcceleration;
  float defaultSmoothAlpha;

  Controller();
  void setup(MotorController* motorController);
  void processInput(byte latestInput);
  bool setMotorPower();
};

#endif
