#include <Arduino.h>
#include "./esp32_digital_led_lib.h"
#include "../../utility/simpletimer.h"
#include "./esp32_digital_led.h"
#include "../../configuration/default/configuration.h"
#include "./esp32_digital_led_config.h"

void Esp32_digital_led::setup()
{
  if (mIsSetup == false)
  {
    mIsSetup = true;
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up "), getModuleName());
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    onEvent(Events::CONFIGURE);
    gpioSetup(mCfg -> pin, OUTPUT, LOW);
    static strand_t STRANDS[] = {{.rmtChannel = 2, .gpioNum = mCfg -> pin, .ledType = LED_WS2812B_V3, .brightLimit = 32, .numPixels =  mCfg -> pixelCount, .pixels = nullptr, ._stateVars = nullptr}};
    if (digitalLeds_initStrands(STRANDS, 1) == -1)
      Logger::Instance().write(LogLevel::WARNING, getModuleName(), FPSTR(" NOT INITIALIZED"));
    mPixels = &STRANDS[0];
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up "), getModuleName());
  }
}

void Esp32_digital_led::loop()
{
  if (enabled())
  {
    if (mSimpleTimer.check())
    {
      Logger::Instance().write(LogLevel::DEBUG, getModuleName(), FPSTR("::loop"));
      if (mLightsEnabled == false)
        reset();
      else if (mIsVehicleDead)
        dead();
      else
      {
        handlePower();
        handleDirection();
      }
      digitalLeds_updatePixels(mPixels);
    }
  }
}

void Esp32_digital_led::reset()
{
  digitalLeds_resetPixels(mPixels);
}

void Esp32_digital_led::dead()
{
  //Add index from-to for DEAD instead of full range
  if (mCfg -> pixelCount > 0)
  {
    byte i;
    static byte c1 = 0;
    static byte c2 = 5;
    for (i = 0; i < mCfg -> pixelCount; i++)
      setPixelColor(i, c1, 0, 0);
    c1 += c2;
    if ((c1 >= 255) || (c1 <= 0))
      c2 = -c2;
  }
}

void Esp32_digital_led::accelerate()
{
  setPixelColor(mCfg -> frontLedStartIndex, mCfg -> frontLedEndIndex, 100, 100, 100);
  setPixelColor(mCfg -> backLedStartIndex, mCfg -> backLedEndIndex, 50, 0, 0);
}

void Esp32_digital_led::brake()
{
  setPixelColor(mCfg -> backLedStartIndex, mCfg -> backLedEndIndex, 255, 0, 0);
}

void Esp32_digital_led::left()
{
  setPixelColor(mCfg -> frontLedStartIndex, 100, 100, 0);
  setPixelColor(mCfg -> backLedEndIndex, 100, 100, 0);
  setPixelColor(mCfg -> leftLedStartIndex, mCfg -> leftLedEndIndex, 100, 100, 0);
}

void Esp32_digital_led::right()
{
  setPixelColor(mCfg -> frontLedEndIndex, 100, 100, 0);
  setPixelColor(mCfg -> backLedStartIndex, 100, 100, 0);
  setPixelColor(mCfg -> rightLedStartIndex, mCfg -> rightLedEndIndex, 100, 100, 0);
}

void Esp32_digital_led::handlePower()
{
  accelerate();
  if (mBrake)
    brake();
}

void Esp32_digital_led::handleDirection()
{
  setPixelColor(mCfg -> leftLedStartIndex, mCfg -> leftLedEndIndex, 0, 0, 0);
  setPixelColor(mCfg -> rightLedStartIndex, mCfg -> rightLedEndIndex, 0, 0, 0);
  if (mTurnTimer.check())
    mBlinkOn = !mBlinkOn;
  if (mBlinkOn)
  {
    if (mLeft)
      left();
    else if (mRight)
      right();
  }
}

void Esp32_digital_led::gpioSetup(int gpioNum, int gpioMode, int gpioVal) {
    gpio_num_t gpioNumNative = static_cast<gpio_num_t>(gpioNum);
    gpio_mode_t gpioModeNative = static_cast<gpio_mode_t>(gpioMode);
    gpio_pad_select_gpio(gpioNumNative);
    gpio_set_direction(gpioNumNative, gpioModeNative);
    gpio_set_level(gpioNumNative, gpioVal);
}

void Esp32_digital_led::command(byte command)
{
  if (enabled())
  {
    Commands comm = static_cast<Commands>(command);
    if (comm == Commands::VEHICLE_DEAD)
    {
      mIsVehicleDead = true;
    }
    else if (comm == Commands::VEHICLE_ALIVE)
    {
      mIsVehicleDead = false;
    }
    else if (!mIsVehicleDead && comm == Commands::CONTROL_NONE)
    {
      mPower = false;
      mBrake = false;
      mLeft = false;
      mRight = false;
    }
    else if (!mIsVehicleDead && comm == Commands::DRIVE_NEUTRAL)
    {
      mPower = false;
      mBrake = false;
    }
    else if (!mIsVehicleDead && comm == Commands::DRIVE_POWER)
    {
      mPower = true;
      mBrake = false;
    }
    else if (!mIsVehicleDead && comm == Commands::DRIVE_BRAKE)
    {
      mPower = false;
      mBrake = true;
    }
    else if (!mIsVehicleDead && comm == Commands::TURN_LEFT)
    {
      mTurnTimer.reset();
      mBlinkOn = true;
      mLeft = true;
      mRight = false;
    }
    else if (!mIsVehicleDead && comm == Commands::TURN_RIGHT)
    {
      mTurnTimer.reset();
      mBlinkOn = true;
      mLeft = false;
      mRight = true;
    }
    else if (!mIsVehicleDead && comm == Commands::TURN_NEUTRAL)
    {
      mLeft = false;
      mRight = false;
    }
    else if (comm == Commands::LED_ENABLED)
    {
      mLightsEnabled = true;
    }
    else if (comm == Commands::LED_DISABLED)
    {
      mLightsEnabled = false;
    }
  }
}

void Esp32_digital_led::onDisable()
{
  reset();
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
pixelColor_t Esp32_digital_led::colorWheel(byte color)
{
  color = 255 - color;
  if (color < 85) {
    return pixelFromRGB(255 - color * 3, 0, color * 3);
  }
  if (color < 170) {
    color -= 85;
    return pixelFromRGB(0, color * 3, 255 - color * 3);
  }
  color -= 170;
  return pixelFromRGB(color * 3, 255 - color * 3, 0);
}

void Esp32_digital_led::setPixelColor(byte index, byte r, byte g, byte b)
{
  if (index >= 0 && index < mCfg -> pixelCount)
  {
    mPixels -> pixels[index] = pixelFromRGB(r, g, b);
  }
}

void Esp32_digital_led::setPixelColor(byte index, byte color)
{
  if (index >= 0 && index < mCfg -> pixelCount)
  {
    mPixels -> pixels[index] = colorWheel(color);
  }
}

void Esp32_digital_led::setPixelColor(byte fromIndex, byte toIndex, byte r, byte g, byte b)
{
  pixelColor_t pc = pixelFromRGB(r, g, b);
  for(byte i=fromIndex; i<=toIndex; i++){
    mPixels -> pixels[i] = pc;
  }
}

void Esp32_digital_led::setPixelColor(byte fromIndex, byte toIndex, byte color)
{
  pixelColor_t pc = colorWheel(color);
  for(byte i=fromIndex; i<=toIndex; i++){
    mPixels -> pixels[i] = pc;
  }
}
