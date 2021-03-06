#include <Arduino.h>
#include <algorithm>
#include <Servo.h>

#include "./pwm_steering.h"
#include "./pwm_steering_wheel_decorator.h"
#include "../../configuration/default/configuration.h"
#include "../base/limit_module.h"
#include "../../utility/tools.h"
#include "../../log/logger.h"

void Pwm_steering::setup() {
  if (mIsSetup == false)
  {
    mIsSetup = true;
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up "), getModuleName());
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    onEvent(Events::CONFIGURE);
    std::vector<IWheel*> wheelArray = mFMV -> getWheelValues();
    for (int i=0; i < wheelArray.size(); i++)
    {
      if (wheelArray[i] -> canSteer())
      {
        //All pin numbers are allowed,but only pins 2,4,12-19,21-23,25-27,32-33 are recommended.
        pinMode(mCfg -> servoPins[wheelDecorators.size()], OUTPUT);
        Servo * mServo = new Servo();
        mServo -> attach(mCfg -> servoPins[wheelDecorators.size()]);
        wheelDecorators.push_back(new Pwm_steering_wheel_decorator(wheelArray[i], mServo, mCfg -> minAngle, mCfg -> neutral, mCfg -> maxAngle));
      }
    }
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up "), getModuleName());
  }
}

void Pwm_steering::loop()
{
  if (enabled())
  {
    if (mSimpleTimer.check())
    {
      IModule * mb = mFMV -> modules().getEnabledByRole(Roles::LIMIT_MODULE);
      if (mb != nullptr)
      {
        Logger::Instance().write(LogLevel::DEBUG, getModuleName(), FPSTR("::loop"));
        Limit_module * ic = static_cast<Limit_module*>(mb);
        mInputControl = Vehiclecontrol(ic -> getOutputControl());
        //Logger::Instance().write(LogLevel::DEBUG, "Pwm_steering::Turning " + String(mInputControl.getLeft()) + " " + String(mInputControl.getRight()));
        for (int i=0; i<wheelDecorators.size(); i++)
        {
          wheelDecorators[i] -> setWheelControl(mInputControl);
        }
      }
    }
  }
}

void Pwm_steering::command(byte command)
{

}

void Pwm_steering::onDisable()
{
  for (int i=0; i<wheelDecorators.size(); i++)
  {
      Logger::Instance().write(LogLevel::DEBUG, "Pwm_steering::onDisable");
      mInputControl.reset();
      wheelDecorators[i] -> setWheelControl(mInputControl);
  }
}
