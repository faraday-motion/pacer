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
    Logger::Instance().write(LogLevel::INFO, FPSTR("Control_priority::Priority1: "), String(mPriority1));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Control_priority::Priority2: "), String(mPriority2));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Control_priority::Priority3: "), String(mPriority3));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Control_priority::Priority4: "), String(mPriority4));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Control_priority::Priority5: "), String(mPriority5));
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
      Control_module * activeControl = mFMV -> modules().getActiveControl();
      if (activeControl != nullptr)
      {
        activeId = activeControl -> id();
        mFMV -> sensors().add("active", activeId);
      }

      Control_module * priority1 = nullptr;
      Control_module * priority2 = nullptr;
      Control_module * priority3 = nullptr;
      Control_module * priority4 = nullptr;
      Control_module * priority5 = nullptr;

      if (mPriority1 != -1)
      {
        priority1 = static_cast<Control_module*>(mFMV -> modules().get(mPriority1));
        if (priority1 != nullptr)
          Logger::Instance().write(LogLevel::DEBUG, FPSTR("Control_priority has priority1: "), String(mPriority1));
      }
      if (mPriority2 != -1)
      {
        priority2 = static_cast<Control_module*>(mFMV -> modules().get(mPriority2));
        if (priority2 != nullptr)
          Logger::Instance().write(LogLevel::DEBUG, FPSTR("Control_priority has priority2: "), String(mPriority2));
      }
      if (mPriority3 != -1)
      {
        priority3 = static_cast<Control_module*>(mFMV -> modules().get(mPriority3));
        if (priority3 != nullptr)
          Logger::Instance().write(LogLevel::DEBUG, FPSTR("Control_priority has priority3: "), String(mPriority3));
      }
      if (mPriority4 != -1)
      {
        priority4 = static_cast<Control_module*>(mFMV -> modules().get(mPriority4));
        if (priority4 != nullptr)
          Logger::Instance().write(LogLevel::DEBUG, FPSTR("Control_priority has priority4: "), String(mPriority4));
      }
      if (mPriority5 != -1)
      {
        priority5 = static_cast<Control_module*>(mFMV -> modules().get(mPriority5));
        if (priority5 != nullptr)
          Logger::Instance().write(LogLevel::DEBUG, FPSTR("Control_priority has priority5: "), String(mPriority5));
      }

      if (priority1 != nullptr && priority1 -> enabled() && priority1 -> hasClient())
      {
        if (mPriority1 != activeId)
        {
          mFMV -> modules().setActiveController(mPriority1);
          onEvent(Events::ACTIVE_PRIORITY1);
          Logger::Instance().write(LogLevel::DEBUG, FPSTR("setActiveController: "), String(mPriority1));
        }
        return;
      }
      else if (priority2 != nullptr && priority2 -> enabled() && priority2 -> hasClient())
      {
        if (mPriority2 != activeId)
        {
          mFMV -> modules().setActiveController(mPriority2);
          onEvent(Events::ACTIVE_PRIORITY2);
          Logger::Instance().write(LogLevel::DEBUG, FPSTR("setActiveController: "), String(mPriority2));
        }
        return;
      }
      else if (priority3 != nullptr && priority3 -> enabled() && priority3 -> hasClient())
      {
        if (mPriority3 != activeId)
        {
          mFMV -> modules().setActiveController(mPriority3);
          onEvent(Events::ACTIVE_PRIORITY3);
          Logger::Instance().write(LogLevel::DEBUG, FPSTR("setActiveController: "), String(mPriority3));
        }
        return;
      }
      else if (priority4 != nullptr && priority4 -> enabled() && priority4 -> hasClient())
      {
        if (mPriority4 != activeId)
        {
          mFMV -> modules().setActiveController(mPriority4);
          onEvent(Events::ACTIVE_PRIORITY4);
          Logger::Instance().write(LogLevel::DEBUG, FPSTR("setActiveController: "), String(mPriority4));
        }
        return;
      }
      else if (priority5 != nullptr && priority5 -> enabled() && priority5 -> hasClient())
      {
        if (mPriority5 != activeId)
        {
          mFMV -> modules().setActiveController(mPriority5);
          onEvent(Events::ACTIVE_PRIORITY5);
          Logger::Instance().write(LogLevel::DEBUG, FPSTR("setActiveController: "), String(mPriority5));
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
