#include "BalanceController.h"

BalanceController::BalanceController(ConfigController* configController, AbstractDevice device)
  : AbstractController(configController, device)
{
}


bool BalanceController::handleController()
{
  // code
  int analogReading = analogRead(A0);
  if (this->previousReading == analogReading)
    sameReadingCount++;
  else
    sameReadingCount = 0;

  int adjustedAnaglogReading = constrain(analogReading, brakeConstraint, accelerateConstraint);
  byte sample = map(adjustedAnaglogReading, brakeConstraint, accelerateConstraint, 0, 100);

  processInput(sample);
  return true;
}

bool BalanceController::enable()
{
  return true;
}

bool BalanceController::disable()
{
  return true;
}
