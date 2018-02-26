#include <Arduino.h>
#include <algorithm>

#include "./pwm_controller.h"
#include "./pwm_controller_wheel_decorator.h"
#include "../../configuration/default/configuration.h"
#include "../base/limit_module.h"
#include "../../utility/tools.h"

void Pwm_controller::setup() {
  if (mIsSetup == false)
  {
    mIsSetup = true;
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up "), getModuleName());
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    //Configure the servos's
    std::vector<Wheel*> wheelArray = mFMV -> getWheelValues();
    for (int i=0; i<wheelArray.size(); i++)
    {
      if (wheelArray[i] -> isElectric())
      {
        //All pin numbers are allowed,but only pins 2,4,12-19,21-23,25-27,32-33 are recommended.
        pinMode(mServoPins[wheelDecorators.size()], OUTPUT);
        Servo* mServo = new Servo();
        mServo -> attach(mServoPins[wheelDecorators.size()]);
        wheelDecorators.push_back(new Pwm_controller_wheel_decorator(wheelArray[i], mServo));
      }
    }
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up "), getModuleName());
  }
}

void Pwm_controller::loop()
{
  if (enabled())
  {
    if (mSimpleTimer.check())
    {
      Modulebase* mb = mFMV -> modules().getEnabledByRole(Roles::LIMIT_MODULE);
      if (mb != nullptr)
      {
        Logger::Instance().write(LogLevel::DEBUG, getModuleName(), FPSTR("::loop"));
        Limit_module* ic = static_cast<Limit_module*>(mb);
        mInputControl = Vehiclecontrol(ic -> getOutputControl());
        //Copy all inputs to the individual powered wheels
        for (int i=0; i<wheelDecorators.size(); i++)
        {
          wheelDecorators[i] -> setWheelControl(mInputControl);
        }
      }
    }
  }
}

void Pwm_controller::command(byte command)
{

}

void Pwm_controller::onDisable()
{
  for (int i=0; i<wheelDecorators.size(); i++)
  {
      Logger::Instance().write(LogLevel::DEBUG, FPSTR("Pwm_controller::onDisable"));
      mInputControl.reset();
      wheelDecorators[i] -> setWheelControl(mInputControl);
  }
}
