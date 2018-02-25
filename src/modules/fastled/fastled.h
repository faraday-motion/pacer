#ifndef FASTLED_H
#define FASTLED_H
#include "../../utility/simpletimer.h"
#include "../base/modulebase.h"
#include "../../fmv.h"

class Fastled : virtual public Modulebase
{
private:
  SimpleTimer mSimpleTimer;
  bool mIsDirty = false;
protected:
  FMV *mFMV;
    void onDisable();
public:
  Fastled(byte id, FMV *fmv, int interval = 25, int critical = 50) : Modulebase(id, Modules::FASTLED)  {
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
