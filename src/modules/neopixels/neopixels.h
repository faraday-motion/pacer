#ifndef NEOPIXELS_H
#define NEOPIXELS_H
#include <Adafruit_NeoPixel.h>
#include "../../configuration/configurator.h"
#include "./neopixels_config.h"
#include "../../utility/simpletimer.h"
#include "../base/modulebase.h"
#include "../../fmv.h"

class Neopixels : virtual public Modulebase
{

public:
  Neopixels(byte id, FMV *fmv, Neopixels_config* cfg = nullptr) : Modulebase(id, Modules::NEOPIXELS)  {
    mFMV = fmv;
    if (cfg == nullptr)
      mCfg = static_cast<Neopixels_config*>(Configurator::Instance().createConfig(id, Configurations::NEOPIXELS_CONFIG));
    else
      mCfg = cfg;
    setConfig();
  }

  enum Commands : byte {
    VEHICLE_DEAD,
    VEHICLE_ALIVE,
    CONTROL_NONE,
    DRIVE_NEUTRAL,
    DRIVE_POWER,
    DRIVE_BRAKE,
    TURN_NEUTRAL,
    TURN_LEFT,
    TURN_RIGHT
  };

  void setConfig()
  {
    mSimpleTimer.setName("Neopixels");
    mSimpleTimer.setInterval(mCfg -> interval, mCfg -> critical);
    mPin = mCfg -> pin;
    mPixelcount = mCfg -> pixelcount;
    mBrightness = mCfg -> brightness;
    mBackLedStartIndex = mCfg -> backLedStartIndex;
    mBackLedEndIndex = mCfg -> backLedEndIndex;
    mLeftLedStartIndex = mCfg -> leftLedStartIndex;
    mLeftLedEndIndex = mCfg -> leftLedEndIndex;
    mFrontLedStartIndex = mCfg -> frontLedStartIndex;
    mFrontLedEndIndex = mCfg -> frontLedEndIndex;
    mRightLedStartIndex = mCfg -> rightLedStartIndex;
    mRightLedEndIndex = mCfg -> rightLedEndIndex;
    mDashboardLedStartIndex = mCfg -> dashboardLedStartIndex;
    mDashboardLedEndIndex = mCfg -> dashboardLedEndIndex;
  }

  void setup();
  void loop();
  void command(byte command);
  void reset();
  void dead();
  void powerNeutral();
  void accelerate();
  void brake();
  void left();
  void right();
  void turnNeutral();
private:
  SimpleTimer mSimpleTimer;
  bool mIsVehicleDead = false;
  byte mPin = 0;
  byte mPixelcount = 0;
  byte mBrightness = 0;
  Adafruit_NeoPixel* mNeopixels;
  Neopixels_config* mCfg = nullptr;
  byte mBackLedStartIndex = 0;
  byte mBackLedEndIndex = 0;
  byte mLeftLedStartIndex = 0;
  byte mLeftLedEndIndex = 0;
  byte mFrontLedStartIndex = 0;
  byte mFrontLedEndIndex = 0;
  byte mRightLedStartIndex = 0;
  byte mRightLedEndIndex = 0;
  byte mDashboardLedStartIndex = 0;
  byte mDashboardLedEndIndex = 0;
  Neopixels::Commands mLastCommand = Commands::VEHICLE_ALIVE;
protected:
  FMV *mFMV;
  void onDisable();
  void onEvent(byte eventId)
  {
    mFMV -> moduleEvent(this, eventId);
  }
};

#endif
