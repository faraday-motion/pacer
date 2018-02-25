
#include <Arduino.h>
#include "./ntp_timeservice.h"
#include "../../configuration/default/configuration.h"
#include "../../utility/tools.h"

void Ntp_timeservice::setup() {
  if (mIsSetup == false)
  {
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up "), getModuleName());
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    configTzTime(mTimeZone.c_str(), mTimeServer.c_str());

    //if (getLocalTime((Configurator::Instance().getTimeInfo()), 10000)) {  // wait up to 10sec to sync
    if (getLocalTime(&mTimeInfo, 2000)) {  // wait up to millis to sync
      mGotDateTime = true;
      Logger::Instance().write(LogLevel::INFO, FPSTR("DateTime: "), Tools::dateTimeString(&mTimeInfo, mDateTimeFormat.c_str()));
    }
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up "), getModuleName());
    mIsSetup = true;
  }
}

void Ntp_timeservice::loop()
{
  if (enabled())
  {
    if (mGotDateTime && mSimpleTimer.check())
    {
      Logger::Instance().write(LogLevel::DEBUG, getModuleName(), FPSTR("::loop"));
      if (getLocalTime(&mTimeInfo)) {
        mFMV -> sensors().add("dt", Tools::dateTimeString(&mTimeInfo, mDateTimeFormat.c_str()));
      }
    }
  }
}

void Ntp_timeservice::getTime()
{
/*
  int	tm_sec;
  int	tm_min;
  int	tm_hour;
  int	tm_mday;
  int	tm_mon;
  int	tm_year;
  int	tm_wday;
  int	tm_yday;
  int	tm_isdst;
*/
  if (getLocalTime(&mTimeInfo)) {
    Logger::Instance().write(LogLevel::DEBUG, FPSTR("Ntp_timeservice::getTime:"), Tools::dateTimeString(&mTimeInfo, mDateTimeFormat.c_str()));
   }
}

void Ntp_timeservice::command(byte command)
{
  if (enabled())
  {
    Commands comm = static_cast<Commands>(command);
    if (comm == Commands::GET_TIME)
    {
      getTime();
    }
  }
}
