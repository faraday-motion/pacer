
#include <Arduino.h>
#include "./ntp_timeservice.h"
#include "../../configuration/default/configuration.h"
#include "../../utility/tools.h"

void Ntp_timeservice::setup() {
  if (mIsSetup == false)
  {
    mIsSetup = true;
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up "), getModuleName());
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    onEvent(Events::CONFIGURE);
    configTzTime(mCfg -> timeZone.c_str(), mCfg -> timeServer.c_str());
    onEvent(Events::CONFIGURE);
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up "), getModuleName());
  }
}

void Ntp_timeservice::loop()
{
  if (enabled())
  {
    if (mSimpleTimer.check())
    {
      Logger::Instance().write(LogLevel::DEBUG, getModuleName(), FPSTR("::loop"));
      updateTime();
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
  if (getLocalTime(&mTimeInfo, 10000)) {  // wait up to 10sec to sync
    mGotTimeInfo = true;
    Logger::Instance().write(LogLevel::DEBUG, FPSTR("DateTime: "), Tools::dateTimeString(&mTimeInfo, mCfg -> dateTimeFormat.c_str()));
    mFMV -> sensors().setStringSensor("dt", Tools::dateTimeString(&mTimeInfo, mCfg -> dateTimeFormat.c_str()));
  }
}

void Ntp_timeservice::updateTime()
{
  if (mGotTimeInfo && getLocalTime(&mTimeInfo))
  {
      time_t now;
      time(&now);
      localtime_r(&now, &mTimeInfo);
      Logger::Instance().write(LogLevel::DEBUG, FPSTR("Ntp_timeservice::updateTime:"), Tools::dateTimeString(&mTimeInfo, mCfg -> dateTimeFormat.c_str()));
      mFMV -> sensors().setStringSensor("dt", Tools::dateTimeString(&mTimeInfo, mCfg -> dateTimeFormat.c_str()));
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
