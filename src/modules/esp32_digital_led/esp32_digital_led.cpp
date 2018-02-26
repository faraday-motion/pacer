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
    gpioSetup(mPin, OUTPUT, LOW);
    mSTRAND = new strand_t;
    mSTRAND -> rmtChannel = 1;
    mSTRAND -> gpioNum = mPin;
    mSTRAND -> ledType = LED_WS2812B_V3;
    mSTRAND -> brightLimit = 32;
    mSTRAND -> numPixels =  mPixelcount;
    mSTRAND -> pixels = nullptr;
    mSTRAND -> _stateVars = nullptr;
    if (digitalLeds_initStrand(mSTRAND) == -1)
      Logger::Instance().write(LogLevel::INFO, FPSTR("NOT INITIALIZED"));
    else
      Logger::Instance().write(LogLevel::INFO, FPSTR("INITIALIZED"));
    //dead();
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up "), getModuleName());
  }
}

void Esp32_digital_led::loop()
{
  if (enabled())
  {
    if (mSimpleTimer.check())
    {
    //  Logger::Instance().write(LogLevel::DEBUG, getModuleName(), FPSTR("::loop"));
      //dead();
      //if (currIdx >= pStrand->numPixels)
      //digitalLeds_resetPixels(strands[i]);
      //delay(1);
      //digitalLeds_updatePixels(&mSTRAND);
    }
  }
}

void Esp32_digital_led::command(byte command)
{
  if (enabled())
  {
    //Logger::Instance().write(LogLevel::DEBUG, "Esp32_digital_led::command: " + (String)command);
    return;
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

void Esp32_digital_led::reset()
{
  //digitalLeds_resetPixels(mPixels);
  pixelColor_t color = pixelFromRGB(0, 0, 0);
  for(byte i=0;i<mPixelcount;i++){
    mSTRAND -> pixels[i] = color;
  }
}

void Esp32_digital_led::dead()
{
  pixelColor_t color = pixelFromRGB(255, 0, 0);
  for(byte i=0;i<mPixelcount;i++){
    mSTRAND -> pixels[i] = color;
  }
}

void Esp32_digital_led::powerNeutral()
{
  for (byte i = mBackLedStartIndex; i <= mBackLedEndIndex; i++)
  {
    mPixels -> pixels[i] = pixelFromRGB(50, 0, 0);
  }
  for (byte i = mFrontLedStartIndex; i <= mFrontLedEndIndex; i++)
  {
    mPixels -> pixels[i] = pixelFromRGB(100, 100, 100);
  }
  for (byte i = mLeftLedStartIndex; i <= mLeftLedEndIndex; i++)
  {
    mPixels -> pixels[i] = pixelFromRGB(0, 0, 0);
  }
  for (byte i = mRightLedStartIndex; i <= mRightLedEndIndex; i++)
  {
    mPixels -> pixels[i] = pixelFromRGB(0, 0, 0);
  }
}

void Esp32_digital_led::accelerate()
{
  for (byte i = mFrontLedStartIndex; i <= mFrontLedEndIndex; i++)
  {
    mPixels -> pixels[i] = pixelFromRGB(100, 100, 100);
  }
  for (byte i = mBackLedStartIndex; i <= mBackLedEndIndex; i++)
  {
    mPixels -> pixels[i] = pixelFromRGB(50, 0, 0);
  }
}

void Esp32_digital_led::brake()
{
  for (byte i = mFrontLedStartIndex; i <= mFrontLedEndIndex; i++)
  {
    mPixels -> pixels[i] = pixelFromRGB(100, 100, 100);
  }
  for (byte i = mBackLedStartIndex; i <= mBackLedEndIndex; i++)
  {
    mPixels -> pixels[i] = pixelFromRGB(255, 0, 0);
  }
}

void Esp32_digital_led::left()
{
  for (byte i = mRightLedStartIndex; i <= mRightLedEndIndex; i++)
  {
    mPixels -> pixels[i] = pixelFromRGB(0, 0, 0);
  }
  for (byte i = mLeftLedStartIndex; i <= mLeftLedEndIndex; i++)
  {
    mPixels -> pixels[i] = pixelFromRGB(100, 100, 0);
  }
}

void Esp32_digital_led::right()
{
  for (byte i = mLeftLedStartIndex; i <= mLeftLedEndIndex; i++)
  {
    mPixels -> pixels[i] = pixelFromRGB(0, 0, 0);
  }
  for (byte i = mRightLedStartIndex; i <= mRightLedEndIndex; i++)
  {
    mPixels -> pixels[i] = pixelFromRGB(100, 100, 0);
  }
}

void Esp32_digital_led::turnNeutral()
{
  for (byte i = mLeftLedStartIndex; i <= mLeftLedEndIndex; i++)
  {
    mPixels -> pixels[i] = pixelFromRGB(0, 0, 0);
  }
  for (byte i = mRightLedStartIndex; i <= mRightLedEndIndex; i++)
  {
    mPixels -> pixels[i] = pixelFromRGB(0, 0, 0);
  }
}


void Esp32_digital_led::gpioSetup(int gpioNum, int gpioMode, int gpioVal) {
    gpio_num_t gpioNumNative = static_cast<gpio_num_t>(gpioNum);
    gpio_mode_t gpioModeNative = static_cast<gpio_mode_t>(gpioMode);
    gpio_pad_select_gpio(gpioNumNative);
    gpio_set_direction(gpioNumNative, gpioModeNative);
    gpio_set_level(gpioNumNative, gpioVal);
}

void Esp32_digital_led::onDisable()
{
  reset();
}
