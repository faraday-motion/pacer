#ifndef FASTLED_LED_H
#define FASTLED_LED_H
#include "../../utility/simpletimer.h"
#include "../base/modulebase.h"
#include "../../fmv.h"

class Fastled_led : public virtual Modulebase
{
private:
  SimpleTimer mSimpleTimer;
  bool mIsDirty = false;
protected:
  IFMV * mFMV;
  void onDisable();
  void onEvent(byte eventId, bool always = false)
  {
    mFMV -> moduleEvent(this, eventId);
  }
public:
  Fastled_led(byte id, IFMV * fmv, int interval = 25, int critical = 50) : Modulebase(id, Modules::FASTLED_LED)  {
    mFMV = fmv;
    mSimpleTimer.setInterval(interval, critical);
  }

  void setConfig()
  {
    //setEnabled(mCfg -> enabled);
  }

  enum Commands : byte {
    VEHICLE_ISDEAD,
    VEHICLE_ISALIVE,
    DRIVE_NEUTRAL,
    DRIVE_POWER,
    DRIVE_BRAKE,
    TURN_NEUTRAL,
    TURN_LEFT,
    TURN_RIGHT
  };

  void setup();
  void loop();
  void command(byte command);
  void reset();
  void neutral();
  void acc(bool enable);
  void brake(bool enable);
  void left(bool enable);
  void right(bool enable);

  String getModuleName()
  {
    return FPSTR("FASTLED");
  }
};

#endif
