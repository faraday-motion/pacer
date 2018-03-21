#ifndef ESP32_DIGITAL_LED_H
#define ESP32_DIGITAL_LED_H
#include "./esp32_digital_led_lib.h"
#include "../../configuration/configurator.h"
#include "./esp32_digital_led_config.h"
#include "../../utility/simpletimer.h"
#include "../base/modulebase.h"
#include "../../interfaces/interfaces.hpp"

class Esp32_digital_led : public Modulebase
{

public:
  Esp32_digital_led(byte id, IFMV * fmv, Esp32_digital_led_config * cfg = nullptr) : Modulebase(id, Modules::ESP32_DIGITAL_LED)  {
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
    TURN_RIGHT,
    LED_ENABLED,
    LED_DISABLED
  };

  void setConfig()
  {
    mSimpleTimer.setName("Esp32_digital_led");
    mSimpleTimer.setInterval(mCfg -> interval, mCfg -> critical);
    mTurnTimer.setInterval(1000);
    setEnabled(mCfg -> enabled);
  }

  void setup();
  void loop();
  void command(byte command);


  String getModuleName()
  {
    return FPSTR("ESP32_DIGITAL_LED");
  }
private:
  strand_t * mPixels;
  SimpleTimer mSimpleTimer;
  SimpleTimer mTurnTimer;
  Esp32_digital_led_config * mCfg = nullptr;
  bool mIsVehicleDead = false;
  bool mLeft = false;
  bool mRight = false;
  bool mPower = false;
  bool mBrake = false;
  bool mBlinkOn = false;
  bool mLightsEnabled = true;
  void gpioSetup(int gpioNum, int gpioMode, int gpioVal);
  void reset();
  void dead();
  void accelerate();
  void brake();
  void left();
  void right();
  void handlePower();
  void handleDirection();
  void setPixelColor(byte index, byte r, byte g, byte b);
  void setPixelColor(byte index, byte color);
  void setPixelColor(byte fromIndex, byte toIndex, byte color);
  void setPixelColor(byte fromIndex, byte toIndex, byte r, byte g, byte b);
  pixelColor_t colorWheel(byte color);
protected:
  IFMV * mFMV = nullptr;
  void onDisable();
  void onEvent(byte eventId, bool always = false)
  {
    mFMV -> moduleEvent(this, eventId);
  }
};

#endif
