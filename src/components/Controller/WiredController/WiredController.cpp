#include <Arduino.h>
#include "WiredController.h"
#include "components/Utility/Log.h"

WiredController::WiredController(AbstractDevice device)
 : AbstractController(device)
{
  Log::Logger()->write(Log::Level::DEBUG, "Started Construction of WiredController: ");
  accelConstraint = device.accelConstraint;
  brakeConstraint = device.brakeConstraint;
  Log::Logger()->write(Log::Level::DEBUG, "Finished Construction of WiredController: ");
}


bool WiredController::handleController()
{
  byte sampleCount = 5;
  int sampleSum = 0;

  for (byte i = 0; i < sampleCount; i++) {
    int sample = analogRead(A0); // TODO:: GPIO should be configured
    sampleSum += sample;
    this->checkSample(sample);
  }

  int sample = sampleSum / sampleCount;
  int adjustedSample = constrain(sample, brakeConstraint, accelConstraint);
  byte s = map(adjustedSample, brakeConstraint, accelConstraint, 0, 100);
  Log::Logger()->write(Log::Level::DEBUG, "WiredController Sample Read: " + (String)sample);

  if (s >= 45 && s <= 55)
    s = 50;

  if (sameReadingCount > 10)
  {
    // Here we should return false in theory. With joystick we generate constantly the same values and that makes us have it disconnected. Which is not what we want. Think of adding a retry to connect to the controller manager or connection manager.s
    processInput(50);
  }
  else
  {
    Log::Logger()->write(Log::Level::DEBUG, "WiredController Sending Value: " + (String)s);
    processInput(s);
  }

  return true;
}

bool WiredController::enable()
{
  return true;
}

bool WiredController::disable()
{
  return true;
}


void WiredController::checkSample(int sample)
{
  if (previousReading == sample)
    sameReadingCount++;
  else
    sameReadingCount = 0;
  previousReading = sample;

}
