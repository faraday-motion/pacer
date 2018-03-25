#ifndef NEOPIXELS_LED_H
#define NEOPIXELS_LED_H
#include <Adafruit_NeoPixel.h>
#include "../../configuration/configurator.h"
#include "./neopixels_led_config.h"
#include "../../utility/simpletimer.h"
#include "../base/modulebase.h"
#include "../../interfaces/interfaces.hpp"

//#include "../../fmv.h"

class Neopixels_led : public virtual Modulebase
{

public:
  Neopixels_led(byte id, IFMV * fmv, Neopixels_led_config * cfg = nullptr) : Modulebase(id, Modules::NEOPIXELS_LED)  {
    mFMV = fmv;
    if (cfg == nullptr)
      mCfg = static_cast<Neopixels_led_config*>(Configurator::Instance().createConfig(id, Configurations::NEOPIXELS_LED_CONFIG));
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

  enum Events : byte {
    CONFIGURE
  };
  
  void setConfig()
  {
    mSimpleTimer.setName("Neopixels");
    mSimpleTimer.setInterval(mCfg -> interval, mCfg -> critical);
    setEnabled(mCfg -> enabled);
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

  String getModuleName()
  {
    return FPSTR("NEOPIXELS");
  }
private:
  SimpleTimer mSimpleTimer;
  bool mIsVehicleDead = false;
  byte mPin = 0;
  byte mPixelcount = 0;
  byte mBrightness = 0;
  Adafruit_NeoPixel * mNeopixels;
  Neopixels_led_config * mCfg = nullptr;
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
protected:
  IFMV * mFMV = nullptr;
  void onDisable();
  void onEvent(byte eventId, bool always = false)
  {
    mFMV -> moduleEvent(this, eventId);
  }
};

#endif
