#include "./analog.h"
#include "../../utility/tools.h"

void Analog::setup() {
  if (mIsSetup == false)
  {
    pinMode(mPin, INPUT);
    readAnalogPin();
    mIsSetup = true;
  }
}

void Analog::loop()
{
  if (enabled())
  {
    if (mSimpleTimer.check()) { // check if the metro has passed its interval .
      readAnalogPin();
    }
  }
}

void Analog::readAnalogPin()
{
    setValue(Tools::analogReadMultiple(mPin, 3));
}
