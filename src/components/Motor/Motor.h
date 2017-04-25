#ifndef Motor_h
#define Motor_h
#include "Arduino.h" // TODO:: optimize the inclusions of the Arduino.h . Here we need if for the byte type to be available.
#include "../MotorController/MotorController.h"

class Motor {
private:
  MotorController* motorController;
  // See what needs to be private and not.
  void adjustPower(byte target);
public:
  // power
  byte defaultInputNeutral;
  byte defaultInputMinBrake;
  byte defaultInputMaxBrake;
  byte defaultInputMinAcceleration;
  byte defaultInputMaxAcceleration;
  float defaultSmoothAlpha;

  //Motor
  byte motorDirection; //0 = Neutral, 1= Acc, 2= Brake, 3 = Reverse
  byte motorTargetPercent;
  byte motorPercent;

  //Current control
  float defaultCurrentNeutral;
  float defaultCurrentAccelerationMax;
  float defaultCurrentAccelerationMin;
  float defaultCurrentBrakeMax;
  float defaultCurrentBrakeMin;

  // Control Variables ported from the Controller Class.
  float controlPower;
  int   controlTarget;

   Motor(MotorController* motorController);
   void setPower(byte target, byte controllerType); //TODO:: set the params types
   void convertPower();
   void setDefaultPower();
   //void setDeadSwitch(); // This should be moved somewhere else. The motor shoudl not be responsible for setting the deadSwitch.

};
#endif
