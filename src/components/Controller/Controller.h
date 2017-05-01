#ifndef Controller_h
#define Controller_h
#include "../MotorController/MotorController.h"
#include "../CurrentController/CurrentController.h"

#include "../ConfigController/Config.h"
#include "../ConfigController/ConfigController.h"

class Controller
{
public:
  // Config
  Config*            config;
  ConfigController*  configController;

  // MotorController
  MotorController*   motorController;
  //CurrentController
  CurrentController  currentController;

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

  Controller(ConfigController* configController);
  void setup(MotorController* motorController);
  void processInput(byte latestInput);
  bool setMotorPower();
};

#endif
