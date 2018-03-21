#include <Arduino.h>
#include "./control_priority.h"
#include "../../configuration/default/configuration.h"
#include "../../configuration/configurator.h"
#include "../../enums/externalcommands.h"

void Control_priority::setup() {
  if (mIsSetup == false)
  {
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up "), getModuleName());
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Control_priority::Priority1: "), String(mCfg -> priority1));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Control_priority::Priority2: "), String(mCfg -> priority2));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Control_priority::Priority3: "), String(mCfg -> priority3));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Control_priority::Priority4: "), String(mCfg -> priority4));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Control_priority::Priority5: "), String(mCfg -> priority5));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up "), getModuleName());
    mIsSetup = true;
  }
}

void Control_priority::loop()
{
  if (enabled())
  {
    if (mSimpleTimer.check())
    {
      Logger::Instance().write(LogLevel::DEBUG, getModuleName(), FPSTR("::loop"));
      int activeId = -1;
      IControlModule * ac = mFMV -> modules().getActiveControl();
      Control_module * activeControl = static_cast<Control_module*>(ac);
      if (activeControl != nullptr)
      {
        activeId = activeControl -> id();
        mFMV -> sensors().setIntSensor("active", activeId);
      }

      Control_module * priority1 = nullptr;
      Control_module * priority2 = nullptr;
      Control_module * priority3 = nullptr;
      Control_module * priority4 = nullptr;
      Control_module * priority5 = nullptr;

      //Lookup all the controllers
      if (mCfg -> priority1 != -1)
      {
        priority1 = static_cast<Control_module*>(mFMV -> modules().get(mCfg -> priority1));
        if (priority1 != nullptr)
          Logger::Instance().write(LogLevel::DEBUG, FPSTR("Control_priority has priority1: "), String(mCfg -> priority1));
      }
      if (mCfg -> priority2 != -1)
      {
        priority2 = static_cast<Control_module*>(mFMV -> modules().get(mCfg -> priority2));
        if (priority2 != nullptr)
          Logger::Instance().write(LogLevel::DEBUG, FPSTR("Control_priority has priority2: "), String(mCfg -> priority2));
      }
      if (mCfg -> priority3 != -1)
      {
        priority3 = static_cast<Control_module*>(mFMV -> modules().get(mCfg -> priority3));
        if (priority3 != nullptr)
          Logger::Instance().write(LogLevel::DEBUG, FPSTR("Control_priority has priority3: "), String(mCfg -> priority3));
      }
      if (mCfg -> priority4 != -1)
      {
        priority4 = static_cast<Control_module*>(mFMV -> modules().get(mCfg -> priority4));
        if (priority4 != nullptr)
          Logger::Instance().write(LogLevel::DEBUG, FPSTR("Control_priority has priority4: "), String(mCfg -> priority4));
      }
      if (mCfg -> priority5 != -1)
      {
        priority5 = static_cast<Control_module*>(mFMV -> modules().get(mCfg -> priority5));
        if (priority5 != nullptr)
          Logger::Instance().write(LogLevel::DEBUG, FPSTR("Control_priority has priority5: "), String(mCfg -> priority5));
      }

      //Select the controller if higher priority
      if (priority1 != nullptr && priority1 -> enabled() && priority1 -> hasClient())
      {
        if (mCfg -> priority1 != activeId)
        {
          mFMV -> modules().setActiveController(mCfg -> priority1);
          onEvent(Events::ACTIVE_PRIORITY1);
          Logger::Instance().write(LogLevel::DEBUG, FPSTR("setActiveController: "), String(mCfg -> priority1));
        }
        return;
      }
      else if (priority2 != nullptr && priority2 -> enabled() && priority2 -> hasClient())
      {
        if (mCfg -> priority2 != activeId)
        {
          mFMV -> modules().setActiveController(mCfg -> priority2);
          onEvent(Events::ACTIVE_PRIORITY2);
          Logger::Instance().write(LogLevel::DEBUG, FPSTR("setActiveController: "), String(mCfg -> priority2));
        }
        return;
      }
      else if (priority3 != nullptr && priority3 -> enabled() && priority3 -> hasClient())
      {
        if (mCfg -> priority3 != activeId)
        {
          mFMV -> modules().setActiveController(mCfg -> priority3);
          onEvent(Events::ACTIVE_PRIORITY3);
          Logger::Instance().write(LogLevel::DEBUG, FPSTR("setActiveController: "), String(mCfg -> priority3));
        }
        return;
      }
      else if (priority4 != nullptr && priority4 -> enabled() && priority4 -> hasClient())
      {
        if (mCfg -> priority4 != activeId)
        {
          mFMV -> modules().setActiveController(mCfg -> priority4);
          onEvent(Events::ACTIVE_PRIORITY4);
          Logger::Instance().write(LogLevel::DEBUG, FPSTR("setActiveController: "), String(mCfg -> priority4));
        }
        return;
      }
      else if (priority5 != nullptr && priority5 -> enabled() && priority5 -> hasClient())
      {
        if (mCfg -> priority5 != activeId)
        {
          mFMV -> modules().setActiveController(mCfg -> priority5);
          onEvent(Events::ACTIVE_PRIORITY5);
          Logger::Instance().write(LogLevel::DEBUG, FPSTR("setActiveController: "), String(mCfg -> priority5));
        }
        return;
      }
    }
  }
}

void Control_priority::command(byte command)
{

}

void Control_priority::onDisable()
{
}
