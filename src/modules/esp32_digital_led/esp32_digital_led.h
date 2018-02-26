#ifndef ESP32_DIGITAL_LED_H
#define ESP32_DIGITAL_LED_H
#include "./esp32_digital_led_lib.h"
#include "../../configuration/configurator.h"
#include "./esp32_digital_led_config.h"
#include "../../utility/simpletimer.h"
#include "../base/modulebase.h"
#include "../../fmv.h"

class Esp32_digital_led : virtual public Modulebase
{

public:
  Esp32_digital_led(byte id, FMV *fmv, Esp32_digital_led_config* cfg = nullptr) : Modulebase(id, Modules::ESP32_DIGITAL_LED)  {
    mFMV = fmv;
    if (cfg == nullptr)
      mCfg = static_cast<Esp32_digital_led_config*>(Configurator::Instance().createConfig(id, Configurations::ESP32_DIGITAL_LED_CONFIG));
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
    mSimpleTimer.setName("Esp32_digital_led");
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
    return FPSTR("ESP32_DIGITAL_LED");
  }
private:
  strand_t * mSTRAND;
  strand_t * mPixels;
  SimpleTimer mSimpleTimer;
  bool mIsVehicleDead = false;
  byte mPin = 0;
  byte mPixelcount = 0;
  byte mBrightness = 0;
  Esp32_digital_led_config * mCfg = nullptr;
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
  Esp32_digital_led::Commands mLastCommand = Commands::VEHICLE_ALIVE;
  void gpioSetup(int gpioNum, int gpioMode, int gpioVal);
protected:
  FMV *mFMV = nullptr;
  void onDisable();
  void onEvent(byte eventId)
  {
    mFMV -> moduleEvent(this, eventId);
  }
};

#endif
