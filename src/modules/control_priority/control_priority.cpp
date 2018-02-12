#include <Arduino.h>
#include "./control_priority.h"
#include "../../configuration/default/configuration.h"
#include "../../configuration/configurator.h"
#include "../../enums/externalcommands.h"

void Control_priority::setup() {
  if (mIsSetup == false)
  {
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up Control_priority"));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up Control_priority"));
    mIsSetup = true;
  }
}

void Control_priority::loop()
{
  if (enabled())
  {
    if (mSimpleTimer.check())
    {
      Logger::Instance().write(LogLevel::DEBUG, FPSTR("Control_priority::loop"));
      int activeId = -1;
      //bool activeHasClient = false;
      //bool activePriority = 0;
      Modulebase* activeControl = mFMV -> modules().getActiveControl();
      if (activeControl != nullptr)
      {
        activeId = activeControl -> id();
        //activeHasClient = activeControl -> hasClient();
        //activePriority = getPriority(activeId);
        //Logger::Instance().write(LogLevel::DEBUG, "activePriority Id: " + String(activeId));
      }

      Modulebase* priority1 = nullptr;
      Modulebase* priority2 = nullptr;
      Modulebase* priority3 = nullptr;
      Modulebase* priority4 = nullptr;
      Modulebase* priority5 = nullptr;

      if (mPriority1 != -1)
      {
        priority1 = mFMV -> modules().get(mPriority1);
      }
      if (mPriority2 != -1)
      {
        priority2 = mFMV -> modules().get(mPriority2);
      }
      if (mPriority3 != -1)
      {
        priority3 = mFMV -> modules().get(mPriority3);
      }
      if (mPriority4 != -1)
      {
        priority4 = mFMV -> modules().get(mPriority4);
      }
      if (mPriority5 != -1)
      {
        priority5 = mFMV -> modules().get(mPriority5);
      }

      if (priority1 != nullptr && priority1 -> hasClient())
      {
        if (mPriority1 != activeId)
        {
          mFMV -> modules().setActiveController(mPriority1);
          onEvent(Events::ACTIVE_PRIORITY1);
          Logger::Instance().write(LogLevel::DEBUG, FPSTR("setActiveController: "), String(mPriority1));
        }
        return;
      }
      else if (priority2 != nullptr && priority2 -> hasClient())
      {
        if (mPriority2 != activeId)
        {
          mFMV -> modules().setActiveController(mPriority2);
          onEvent(Events::ACTIVE_PRIORITY2);
          Logger::Instance().write(LogLevel::DEBUG, FPSTR("setActiveController: "), String(mPriority2));
        }
        return;
      }
      else if (priority3 != nullptr && priority3 -> hasClient())
      {
        if (mPriority3 != activeId)
        {
          mFMV -> modules().setActiveController(mPriority3);
          onEvent(Events::ACTIVE_PRIORITY3);
          Logger::Instance().write(LogLevel::DEBUG, FPSTR("setActiveController: "), String(mPriority3));
        }
        return;
      }
      else if (priority4 != nullptr && priority4 -> hasClient())
      {
        if (mPriority4 != activeId)
        {
          mFMV -> modules().setActiveController(mPriority4);
          onEvent(Events::ACTIVE_PRIORITY4);
          Logger::Instance().write(LogLevel::DEBUG, FPSTR("setActiveController: "), String(mPriority4));
        }
        return;
      }
      else if (priority5 != nullptr && priority5 -> hasClient())
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
