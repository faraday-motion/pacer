#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "../../utility/simpletimer.h"
#include "./neopixels.h"
#include "../../configuration/default/configuration.h"
#include "./neopixels_config.h"

void Neopixels::setup()
{
  if (mIsSetup == false)
  {
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up Neopixels"));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "),String(ESP.getFreeHeap()));
    pinMode(mPin, OUTPUT);
    mNeopixels = new Adafruit_NeoPixel(mPixelcount, mPin, NEO_GRB + NEO_KHZ800);
    mNeopixels -> setBrightness(mBrightness);
    mNeopixels -> begin();
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up Neopixels"));
    mIsSetup = true;
  }
}

void Neopixels::loop()
{
  if (enabled())
  {
    if (mSimpleTimer.check())
    {
      Logger::Instance().write(LogLevel::DEBUG, FPSTR("Neopixels::loop"));
      //The delay is needed in order to avoid flickering on esp32, as the interrupts are giving issues.
      //Might have to change to another library to fix this issue.
      delay(1);
      mNeopixels -> show();
      delay(1);
    }
  }
}

void Neopixels::command(byte command)
{
  if (enabled())
  {
    //Logger::Instance().write(LogLevel::DEBUG, "recievecommand: " + (String)command);
    Commands comm = static_cast<Commands>(command);
    mLastCommand = comm;
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

void Neopixels::reset()
{
  mNeopixels -> clear();
}

void Neopixels::dead()
{
  for(byte i=0;i<mPixelcount;i++){
    mNeopixels -> setPixelColor(i, mNeopixels -> Color(255,0,0));
  }
}

void Neopixels::powerNeutral()
{
  for (byte i = mBackLedStartIndex; i <= mBackLedEndIndex; i++)
  {
    mNeopixels -> setPixelColor(i, mNeopixels -> Color(50,0,0));
  }
  for (byte i = mFrontLedStartIndex; i <= mFrontLedEndIndex; i++)
  {
    mNeopixels -> setPixelColor(i, mNeopixels -> Color(100,100,100));
  }
  for (byte i = mLeftLedStartIndex; i <= mLeftLedEndIndex; i++)
  {
    mNeopixels -> setPixelColor(i, mNeopixels -> Color(0,0,0));
  }
  for (byte i = mRightLedStartIndex; i <= mRightLedEndIndex; i++)
  {
    mNeopixels -> setPixelColor(i, mNeopixels -> Color(0,0,0));
  }
}

void Neopixels::accelerate()
{
  for (byte i = mFrontLedStartIndex; i <= mFrontLedEndIndex; i++)
  {
    mNeopixels -> setPixelColor(i, mNeopixels -> Color(100,100,100));
  }
  for (byte i = mBackLedStartIndex; i <= mBackLedEndIndex; i++)
  {
    mNeopixels -> setPixelColor(i, mNeopixels -> Color(50,0,0));
  }
}

void Neopixels::brake()
{
  for (byte i = mFrontLedStartIndex; i <= mFrontLedEndIndex; i++)
  {
    mNeopixels -> setPixelColor(i, mNeopixels -> Color(100,100,100));
  }
  for (byte i = mBackLedStartIndex; i <= mBackLedEndIndex; i++)
  {
    mNeopixels -> setPixelColor(i, mNeopixels -> Color(255,0,0));
  }
}

void Neopixels::left()
{
  for (byte i = mRightLedStartIndex; i <= mRightLedEndIndex; i++)
  {
    mNeopixels -> setPixelColor(i, mNeopixels -> Color(0,0,0));
  }
  for (byte i = mLeftLedStartIndex; i <= mLeftLedEndIndex; i++)
  {
    mNeopixels -> setPixelColor(i, mNeopixels -> Color(100,100,0));
  }
}

void Neopixels::right()
{
  for (byte i = mLeftLedStartIndex; i <= mLeftLedEndIndex; i++)
  {
    mNeopixels -> setPixelColor(i, mNeopixels -> Color(0,0,0));
  }
  for (byte i = mRightLedStartIndex; i <= mRightLedEndIndex; i++)
  {
    mNeopixels -> setPixelColor(i, mNeopixels -> Color(100,100,0));
  }
}

void Neopixels::turnNeutral()
{
  for (byte i = mLeftLedStartIndex; i <= mLeftLedEndIndex; i++)
  {
    mNeopixels -> setPixelColor(i, mNeopixels -> Color(0,0,0));
  }
  for (byte i = mRightLedStartIndex; i <= mRightLedEndIndex; i++)
  {
    mNeopixels -> setPixelColor(i, mNeopixels -> Color(0,0,0));
  }
}

void Neopixels::onDisable()
{
  reset();
  mNeopixels -> show();
}
