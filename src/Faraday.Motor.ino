
void setPower(byte target, byte controllerType)
{
#if defined(ENABLEDEVMODE)
  Serial.print("setPower:");
  Serial.println(target);
#endif
#if defined(ENABLEWEBUPDATE)
    disableWebUpdate();
#endif

  if (isControllerEnabled())
  {
    metroControllerCommunication.reset();

    //If current type control is nothing, and we get a power command from a controller, set the control type

    if (controlType == 0 && controllerType != 0)
      controlType = controllerType;

    adjustPower(constrain(target, defaultInputMaxBrake, defaultInputMaxAcceleration));
  }
  else
  {
    //Just ensure that we go nowhere
    setDefaultPower();
  }
}

bool isControllerEnabled()
{
  return (controlDead == false && controlEnabled == true);
}

void adjustPower(byte target)
{
  controlTarget = target;
#if defined(ENABLEDEVMODE)
  Serial.print("adjustPowerTarget:");
  Serial.println(target);
#endif
#if defined(ENABLESMOOTHING)
  //Smooth the input
  float targetAlpha = (defaultSmoothAlpha * target) + ((1 - defaultSmoothAlpha) * controlPower);
  //If the value is close to target, set it to target
  if (abs(float(target) - float(targetAlpha)) <= 1)
    targetAlpha = target;
  target = targetAlpha;
#endif

#if defined(ENABLEDEVMODE)
  Serial.print("target:");
  Serial.println(target);
#endif
  if (target > defaultInputMinBrake && target < defaultInputMinAcceleration)
  {
    //When within the inner boundary, set the default neutral value
    target = defaultInputNeutral;
  }
  if (controlCruiseControl == true && controlPower >= defaultInputMinAcceleration)
  {
    //Set input to current output when on cruisecontrol and we are accelerating
    //Not set the controlpower and ignore the power input, maybe needs changing when using nunchuk/reading values from motorcontroller
  }
  else
  {
    controlPower = constrain(target, defaultInputMaxBrake, defaultInputMaxAcceleration);
#if defined(ENABLEDEVMODE)
    Serial.print("adjustPowerActual:");
    Serial.println(controlPower);
#endif
  }
}

/// <summary>
/// Converts the outCurrentPower to the needed servopower
/// </summary>
void convertPower()
{
#if defined(ENABLEDEVMODE)
  Serial.print("convertPower:");
  Serial.println(controlPower);
#endif

  if (isControllerEnabled() == false || (controlPower > defaultInputMinBrake && controlPower < defaultInputMinAcceleration))
  {
    //Neutral
    motorDirection = 0;
    motorPercent  = 0;
    motorTargetPercent  = 0;
#if defined(ENABLEVESC)
    vesc.set_current_brake(0);
#endif

#if defined(ENABLESERVOESC)
    servoESC.writeMicroseconds(servoNeutralPWM);
#endif
  }
  else if (controlPower >= defaultInputMinAcceleration)
  {
    //Accelerating
    motorDirection = 1;
    motorTargetPercent = map(controlTarget, defaultInputMinBrake, defaultInputMaxAcceleration, 0, 100);
#if defined(ENABLEDEVMODE)
    Serial.print("motorTargetPercent:");
    Serial.println(motorTargetPercent);
#endif
    motorPercent = map(controlPower, defaultInputMinBrake, defaultInputMaxAcceleration, 0, 100);

#if defined(ENABLEVESC)
    float motorCurrent = map(controlPower, defaultInputMinAcceleration, defaultInputMaxAcceleration, defaultCurrentNeutral, defaultCurrentAccelerationMax);
    //Hack function for making things more smooth
    float adjustedCurrent = ((motorCurrent * motorCurrent) / defaultCurrentAccelerationMax) + defaultCurrentAccelerationMin;
    adjustedCurrent = constrain(adjustedCurrent, defaultCurrentAccelerationMin, defaultCurrentAccelerationMax);
    vesc.set_current(adjustedCurrent);
#if defined(ENABLEDEVMODE)
    Serial.print("AccelerationCur: ");
    Serial.println(adjustedCurrent);
#endif
#endif

#if defined(ENABLESERVOESC)
    int servoMotorPWM = map(controlPower, defaultInputMinAcceleration, defaultInputMaxAcceleration, servoNeutralPWM, servoMaxPWM);
    servoESC.writeMicroseconds(servoMotorPWM);
#if defined(ENABLEDEVMODE)
    Serial.print("AccelerationServo: ");
    Serial.println(servoMotorPWM);
#endif
#endif

  }
  else
  {
    //Braking
    motorDirection = 2;
    motorTargetPercent = map(controlTarget, defaultInputMinBrake, defaultInputMaxBrake, 0, 100);
#if defined(ENABLEDEVMODE)
    Serial.print("motorTargetPercent:");
    Serial.println(motorTargetPercent);
#endif
    motorPercent  = map(controlPower, defaultInputMinBrake, defaultInputMaxBrake, 0, 100);

#if defined(ENABLEVESC)
    float motorCurrent = map(controlPower, defaultInputMinBrake, defaultInputMaxBrake, defaultCurrentBrakeMin, defaultCurrentBrakeMax);
    float adjustedCurrent = motorCurrent;
#if defined(ENABLENONLINEARBRAKE)    
    adjustedCurrent = ((motorCurrent * motorCurrent) / defaultCurrentBrakeMax) + defaultCurrentBrakeMin;
    adjustedCurrent = constrain(adjustedCurrent, defaultCurrentBrakeMin, defaultCurrentBrakeMax);
#endif    
    vesc.set_current_brake(adjustedCurrent);
#if defined(ENABLEDEVMODE)
    Serial.print("BrakeCur: ");
    Serial.println(adjustedCurrent);
#endif
#endif

#if defined(ENABLESERVOESC)
    int servoMotorPWM = map(controlPower, defaultInputMinBrake, defaultInputMaxBrake, servoNeutralPWM, servoMinPWM);
    servoESC.writeMicroseconds(servoMotorPWM);
#if defined(ENABLEDEVMODE)
    Serial.print("BrakeServo: ");
    Serial.println(servoMotorPWM);
#endif
#endif
  }
}

void setDefaultPower()
{
  Serial.println("setDefaultPower");
  controlEnabled = false;
  controlType = 0;
  motorDirection = 0;
  motorPercent  = 0;
  motorTargetPercent = 0;
  controlTarget = defaultInputNeutral;
  controlPower = defaultInputNeutral;
#if defined(ENABLEVESC)
  vesc.set_current(defaultCurrentNeutral);
#endif

#if defined(ENABLESERVOESC)
  servoESC.writeMicroseconds(servoNeutralPWM);
#endif
}

#if defined(ENABLEDEADSWITCH)
void setDeadSwitch()
{
  controlDead = true;
}
#endif

