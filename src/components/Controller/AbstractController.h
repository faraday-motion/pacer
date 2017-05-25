#ifndef AbstractController_h
#define AbstractController_h
#include "../MotorController/MotorController.h"
#include "../CurrentController/CurrentController.h"

#include "../ConfigController/Config.h"
#include "../ConfigController/ConfigController.h"

//https://en.wikibooks.org/wiki/C%2B%2B_Programming/Classes/Abstract_Classes
class AbstractController
{
private:
  bool setMotorPower();

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

  //Controller Identification
  byte controllerType;
  byte controllerId[5];

  // Controller Constraints
  byte defaultInputNeutral;
  byte defaultInputMinBrake;
  byte defaultInputMaxBrake;
  byte defaultInputMinAcceleration;
  byte defaultInputMaxAcceleration;
  float defaultSmoothAlpha;

  AbstractController(ConfigController* configController, byte controllerType, byte controllerId[]);
  virtual ~AbstractController() {}
  void setup();
  void processInput(byte latestInput);

  virtual void handleController() = 0; // virtual loop function

  // TODO:: These don't really need to be virtual at least for now.
  virtual void read() = 0; // virutal read function
  virtual void write() = 0; // virtaul write function



};

#endif
