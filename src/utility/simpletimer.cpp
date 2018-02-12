#include <Arduino.h>
#include "./simpletimer.h"
#include "../logs/logger.h"

SimpleTimer::SimpleTimer()
{
	reset();
}

SimpleTimer::SimpleTimer(String name)
{
	mName = name;
	reset();
}


SimpleTimer::SimpleTimer(unsigned long interval, unsigned long critical)
{
	mInterval = interval;
	mCritical = critical;
	reset();
}

void SimpleTimer::setInterval(unsigned long interval, unsigned long critical)
{
  mInterval = interval;
	mCritical = critical;
	reset();
}

void SimpleTimer::setName(String name)
{
	mName = name;
}

String SimpleTimer::getName() const
{
	return mName;
}

unsigned long SimpleTimer::getInterval() const
{
	return mInterval;
}

unsigned long SimpleTimer::getCritical() const
{
	return mCritical;
}

bool SimpleTimer::check()
{
	unsigned long actual;
  unsigned long now = millis();
	actual = (now - mPrevious);
  if (mAllowEnter && actual >= mInterval)
	{
		mAllowEnter = false;
		mPrevious = millis();
		mActual = actual;
		if (mCritical > 0 && actual > mCritical)
		{
			Logger::Instance().write(LogLevel::WARNING, mName + " simpleTimer should be " + String(mInterval) + " is " + String(actual));
			mIsOnTime = false;
		}
		else
			mIsOnTime = true;
		mAllowEnter = true;
    return true;
  }
  return false;
}

unsigned long SimpleTimer::getActual() const
{
  return mActual;
}

void SimpleTimer::reset()
{
	//Logger::Instance().write(LogLevel::WARNING, mName + " reset " + String(millis()));
  mPrevious = millis();
}

bool SimpleTimer::onTime()
{
  return mIsOnTime;
}
