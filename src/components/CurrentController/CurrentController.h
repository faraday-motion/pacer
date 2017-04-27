#ifndef CurrentController_h
#define CurrentController_h
#include "Arduino.h" // TODO:: optimize the inclusions of the Arduino.h . Here we need if for the byte type to be available.

class CurrentController {
public:
  // Current control
  //TODO:: This Needs to Come from a Config File.
  float defaultCurrentNeutral;
  float defaultCurrentAccelerationMax;
  float defaultCurrentAccelerationMin;
  float defaultCurrentBrakeMax;
  float defaultCurrentBrakeMin;

  //TODO:: this needs to be coming out of a config file.
  byte defaultInputNeutral;
  byte defaultInputMinBrake;
  byte defaultInputMaxBrake;
  byte defaultInputMinAcceleration;
  byte defaultInputMaxAcceleration;
  float defaultSmoothAlpha;


  CurrentController();
  float getMotorAccelerationCurrent(float previousControllerInput);
  float getMotorBrakingCurrent(float previousControllerInput);
  float getNeutralCurrent();
};

#endif
