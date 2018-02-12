#ifndef SIMPLETIMER_H
#define SIMPLETIMER_H
#include <inttypes.h>
#include <Arduino.h>

class SimpleTimer
{
  private:
    unsigned long mPrevious = 0;
    unsigned long mInterval = 1000;
    unsigned long mCritical = 0;
    unsigned long mActual = 0;
    bool mIsOnTime = false;
    bool mAllowEnter = true;
    String mName = "";
  public:
    SimpleTimer();
    SimpleTimer(String name);
    SimpleTimer(unsigned long interval, unsigned long critical = 0);
    void setInterval(unsigned long interval, unsigned long critical = 0);
    unsigned long getActual() const;
    unsigned long getInterval() const;
    unsigned long getCritical() const;
    bool check();
    void reset();
    bool onTime();
    void setName(String name);
    String getName() const;
};

#endif
