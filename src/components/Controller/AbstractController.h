#ifndef AbstractController_h
#define AbstractController_h
#include "../ConfigController/Config.h"
#include "components/Device/AbstractDevice.h"
#include "components/MotorController/MotorController.h"
#include "components/CurrentController/CurrentController.h"

//https://en.wikibooks.org/wiki/C%2B%2B_Programming/Classes/Abstract_Classes
class AbstractController
{
private:
  // Controller Constraints
  byte defaultInputNeutral;
  byte defaultInputMinBrake;
  byte defaultInputMaxBrake;
  byte defaultInputMinAcceleration;
  byte defaultInputMaxAcceleration;
  float defaultSmoothAlpha;

  // Controller Inputs
  byte  latestInput;
  float previousInput;

  bool setMotorPower();

public:
  // Config
  Config*            config;
  // MotorController
  MotorController*   motorController;
  //Controller Identification
  AbstractDevice controller; // used by the controllerManager
  //CurrentController
  CurrentController  currentController;

  AbstractController(ConfigController* configController, AbstractDevice device);

  void setup();
  void processInput(byte latestInput);

  virtual bool handleController() = 0; // virtual loop function
  virtual bool enable() = 0;  // Let the physical controller that it is enabled.
  virtual bool disable() = 0; // Let the physical controller that it is disabled.
};

#endif
