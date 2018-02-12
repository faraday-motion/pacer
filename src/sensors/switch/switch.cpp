#include "switch.h"

void Switch::setup() {
  if (mIsSetup == false)
  {
    pinMode(mPin, INPUT_PULLDOWN);
    readSwitchPin();
    mIsSetup = true;
  }
}

void Switch::loop()
{
  if (enabled())
  {
    if (mSimpleTimer.check()) { // check if the metro has passed its interval .
      readSwitchPin();
    }
  }
}

void Switch::readSwitchPin()
{
  if (!mInvert)
    setValue(!Tools::digitalReadMultiple(mPin, 3));
  else
    setValue(Tools::digitalReadMultiple(mPin, 3));
}
