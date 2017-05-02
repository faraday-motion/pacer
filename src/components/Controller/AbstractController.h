#ifndef AbstractController_h
#define AbstractController_h
#include "../MotorController/MotorController.h"
#include "../CurrentController/CurrentController.h"

#include "../ConfigController/Config.h"
#include "../ConfigController/ConfigController.h"


//TODO:: this class needs to be abstracted.
//TODO:: read this https://en.wikibooks.org/wiki/C%2B%2B_Programming/Classes/Abstract_Classes
//TODO:: the ControllerManager should handle the setting of activeControlelrs. It should also register the controllers by type.
//TODO:: Now the AbstractController is not really useful. Read "Abstract base classes" -> http://www.cplusplus.com/doc/tutorial/polymorphism/
class AbstractController
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

  //Controller Identification
  byte controllerType;
  byte controllerId;

  // Controller Constraints
  byte defaultInputNeutral;
  byte defaultInputMinBrake;
  byte defaultInputMaxBrake;
  byte defaultInputMinAcceleration;
  byte defaultInputMaxAcceleration;
  float defaultSmoothAlpha;

  AbstractController(ConfigController* configController, MotorController* motorController, byte controllerType, byte controllerId);
  virtual ~AbstractController() {}
  void setup();
  void processInput(byte latestInput);
  bool setMotorPower();
  virtual void read() = 0;


};

#endif
