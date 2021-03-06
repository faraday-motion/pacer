#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "../../utility/simpletimer.h"
#include "./neopixels_led.h"
#include "../../configuration/default/configuration.h"
#include "./neopixels_led_config.h"

void Neopixels_led::setup()
{
  if (mIsSetup == false)
  {
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up "), getModuleName());
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    onEvent(Events::CONFIGURE);
    pinMode(mCfg -> pin, OUTPUT);
    mNeopixels = new Adafruit_NeoPixel(mCfg -> pixelCount, mCfg -> pin, NEO_GRB + NEO_KHZ800);
    mNeopixels -> setBrightness(mCfg -> brightness);
    mNeopixels -> begin();
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up "), getModuleName());
    mIsSetup = true;
  }
}

void Neopixels_led::loop()
{
  if (enabled())
  {
    if (mSimpleTimer.check())
    {
      Logger::Instance().write(LogLevel::DEBUG, getModuleName(), FPSTR("::loop"));
      //The delay is needed in order to avoid flickering on esp32, as the interrupts are giving issues.
      delay(1);
      mNeopixels -> show();
      delay(1);
    }
  }
}

void Neopixels_led::command(byte command)
{
  if (enabled())
  {
    Logger::Instance().write(LogLevel::INFO, FPSTR("Neopixels::command: "), String(command));
    Commands comm = static_cast<Commands>(command);
    if (comm == Commands::VEHICLE_DEAD)
    {
      mIsVehicleDead = true;
      dead();
    }
    else if (comm == Commands::VEHICLE_ALIVE)
    {
      mIsVehicleDead = false;
      reset();
    }
    else if (!mIsVehicleDead && comm == Commands::CONTROL_NONE)
    {
      reset();
    }
    else if (!mIsVehicleDead && comm == Commands::DRIVE_NEUTRAL)
    {
      powerNeutral();
    }
    else if (!mIsVehicleDead && comm == Commands::DRIVE_POWER)
    {
      accelerate();
    }
    else if (!mIsVehicleDead && comm == Commands::DRIVE_BRAKE)
    {
      brake();
    }
    else if (!mIsVehicleDead && comm == Commands::TURN_LEFT)
    {
      left();
    }
    else if (!mIsVehicleDead && comm == Commands::TURN_RIGHT)
    {
      right();
    }
    else if (!mIsVehicleDead && comm == Commands::TURN_NEUTRAL)
    {
      turnNeutral();
    }
  }
}

void Neopixels_led::reset()
{
  mNeopixels -> clear();
}

void Neopixels_led::dead()
{
  for(byte i=0;i<mCfg -> pixelCount;i++){
    mNeopixels -> setPixelColor(i, mNeopixels -> Color(255,0,0));
  }
}

void Neopixels_led::powerNeutral()
{
  for (byte i = mCfg -> backLedStartIndex; i <= mCfg -> backLedEndIndex; i++)
  {
    mNeopixels -> setPixelColor(i, mNeopixels -> Color(50,0,0));
  }
  for (byte i = mCfg -> frontLedStartIndex; i <= mCfg -> frontLedEndIndex; i++)
  {
    mNeopixels -> setPixelColor(i, mNeopixels -> Color(100,100,100));
  }
  for (byte i = mCfg -> leftLedStartIndex; i <= mCfg -> leftLedEndIndex; i++)
  {
    mNeopixels -> setPixelColor(i, mNeopixels -> Color(0,0,0));
  }
  for (byte i = mCfg -> rightLedStartIndex; i <= mCfg -> rightLedEndIndex; i++)
  {
    mNeopixels -> setPixelColor(i, mNeopixels -> Color(0,0,0));
  }
}

void Neopixels_led::accelerate()
{
  for (byte i = mCfg -> frontLedStartIndex; i <= mCfg -> frontLedEndIndex; i++)
  {
    mNeopixels -> setPixelColor(i, mNeopixels -> Color(100,100,100));
  }
  for (byte i = mCfg -> backLedStartIndex; i <= mCfg -> backLedEndIndex; i++)
  {
    mNeopixels -> setPixelColor(i, mNeopixels -> Color(50,0,0));
  }
}

void Neopixels_led::brake()
{
  for (byte i = mCfg -> frontLedStartIndex; i <= mCfg -> frontLedEndIndex; i++)
  {
    mNeopixels -> setPixelColor(i, mNeopixels -> Color(100,100,100));
  }
  for (byte i = mCfg -> backLedStartIndex; i <= mCfg -> backLedEndIndex; i++)
  {
    mNeopixels -> setPixelColor(i, mNeopixels -> Color(255,0,0));
  }
}

void Neopixels_led::left()
{
  for (byte i = mCfg -> rightLedStartIndex; i <= mCfg -> rightLedEndIndex; i++)
  {
    mNeopixels -> setPixelColor(i, mNeopixels -> Color(0,0,0));
  }
  for (byte i = mCfg -> leftLedStartIndex; i <= mCfg -> leftLedEndIndex; i++)
  {
    mNeopixels -> setPixelColor(i, mNeopixels -> Color(100,100,0));
  }
}

void Neopixels_led::right()
{
  for (byte i = mCfg -> leftLedStartIndex; i <= mCfg -> leftLedEndIndex; i++)
  {
    mNeopixels -> setPixelColor(i, mNeopixels -> Color(0,0,0));
  }
  for (byte i = mCfg -> rightLedStartIndex; i <= mCfg -> rightLedEndIndex; i++)
  {
    mNeopixels -> setPixelColor(i, mNeopixels -> Color(100,100,0));
  }
}

void Neopixels_led::turnNeutral()
{
  for (byte i = mCfg -> leftLedStartIndex; i <= mCfg -> leftLedEndIndex; i++)
  {
    mNeopixels -> setPixelColor(i, mNeopixels -> Color(0,0,0));
  }
  for (byte i = mCfg -> rightLedStartIndex; i <= mCfg -> rightLedEndIndex; i++)
  {
    mNeopixels -> setPixelColor(i, mNeopixels -> Color(0,0,0));
  }
}

void Neopixels_led::onDisable()
{
  reset();
  mNeopixels -> show();
}
