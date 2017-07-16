#include <Arduino.h>
#include "WiredController.h"

WiredController::WiredController(ConfigController* configController, RadioDevice device)
 : AbstractController(configController, device)
{

}


bool WiredController::handleController()
{
  byte measurements = 5;
  float totalMeasurement = 0;

  for (byte i = 0; i < measurements; i++)
  {
    totalMeasurement += analogRead(A0);
  }

  float m = totalMeasurement / measurements;

  Serial.print("Raw measrurement. ");
  Serial.println(m);

  m = m / 4;
  byte s = map(m, 48, 200, 0, 100);
  if (s >= 45 && s <= 55)
  {
    s = 50;
  }
  Serial.print("WIRE CONTROLLER INPUT:: ");
  Serial.println(s);
  processInput(s);

  return true;

}

bool WiredController::enable()
{
  return true;
}

void WiredController::read()
{
  return;
}

void WiredController::write()
{
  return;
}
