#include "AbstractController.h"
#include "components/Utility/Log.h"


AbstractController::AbstractController(AbstractDevice device)
{
  Log::Logger()->write(Log::Level::DEBUG, "Started Construction of AbstractController: ");
  this->config = Config::get();
  this->motorController = new MotorController; // Question:: Does this mean I have a new motorController object for each instance of the AbstractController?
  this->motorController->setup();

  this->controller = device; // TODO:: why do we store the device on the abstract and the concrete controller?
  this->setup();
  Log::Logger()->write(Log::Level::DEBUG, "Finished Construction of AbstractController: ");
}

void AbstractController::setup()
{
  // Setting the pointers to Motor and Wifi.
  //this->motorController = motorController;
  Log::Logger()->write(Log::Level::DEBUG, "Setting up the AbstractController...");
  currentController.setup();

  // Seting the default values and states for the controller;
  defaultInputNeutral         = config->controller.defaultInputNeutral;
  defaultInputMinBrake        = config->controller.defaultInputMinBrake;
  defaultInputMaxBrake        = config->controller.defaultInputMaxBrake;
  defaultInputMinAcceleration = config->controller.defaultInputMinAcceleration;
  defaultInputMaxAcceleration = config->controller.defaultInputMaxAcceleration;
  defaultSmoothAlpha          = config->controller.defaultSmoothAlpha;

  // Setting inputs to neutral;
  latestInput                 = defaultInputNeutral;
  previousInput               = defaultInputNeutral;

  // Metro Timer
  this->checkIfInMotion = new Metro(15000);
  this->checkIfInMotion->reset();

  Log::Logger()->write(Log::Level::DEBUG, "Finished Setting up the AbstractController...");

}

// Decides the state of the motor
bool AbstractController::setMotorPower()
{

  if (previousInput > defaultInputMinBrake && previousInput < defaultInputMinAcceleration)
  {
    // we are in neutral.
    float motorCurrent = currentController.getNeutralCurrent();
    motorController->set_current(motorCurrent);
    // For more the x seconds we've been in neutral. We are not in motion anymore.
    if (this->checkIfInMotion->check() == 1) {
      this->isInMotion = false;
    }

    return 0;
  }

  if (previousInput >= defaultInputMinAcceleration)
  {
    //accelerating
    float motorCurrent = currentController.getMotorAccelerationCurrent(previousInput);
    motorController->set_current(motorCurrent);
    this->checkIfInMotion->reset(); // We are in motion. Reset the time watch.
    this->isInMotion = true;
    return 1;
  }

  // braking;
  float motorCurrent = currentController.getMotorBrakingCurrent(previousInput);
  motorController->set_current_brake(motorCurrent);
  this->checkIfInMotion->reset(); // We are in motion. Reset the time watch.
  this->isInMotion = true;
  return 1;
}

void AbstractController::processInput(byte latestInput)
{
  float targetAlpha = (defaultSmoothAlpha * latestInput) + ((1 - defaultSmoothAlpha * previousInput));
  // If the value is close to target, set it to target
  if(abs(float(latestInput) - float(targetAlpha)))
  {
    targetAlpha = latestInput;
  }
  if (latestInput > defaultInputMinBrake && latestInput < defaultInputMinAcceleration)
  {
    // When within the neutral boundary, set the default neutral value
    latestInput = defaultInputNeutral;
  }
  previousInput = constrain(latestInput, defaultInputMaxBrake, defaultInputMaxAcceleration);
  this->setMotorPower();
}
