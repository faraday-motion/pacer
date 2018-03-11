#include <Arduino.h>
#define FASTLED_FORCE_SOFTWARE_SPI
#include <FastLED.h>
//FASTLED_USING_NAMESPACE;
#include "../../utility/simpletimer.h"
#include "./fastled_led.h"
#include "../../configuration/default/configuration.h"

CRGB leds[FASTLED_PIXELS];

void Fastled_led::setup()
{
  if (mIsSetup == false)
  {
    mIsSetup = true;
    Logger::Instance().write(LogLevel::INFO, "Setting up Fastled");
    pinMode(PIN_FASTLED, OUTPUT);
    // set master brightness control
    FastLED.addLeds<NEOPIXEL, PIN_FASTLED>(leds, FASTLED_PIXELS);
    FastLED.setBrightness(100);
    reset();
    neutral();
    Logger::Instance().write(LogLevel::INFO, "Finished setting up Fastled");
  }
}

void Fastled_led::loop()
{
  if (enabled())
  {
    if (mIsDirty){
      //pixels.show();
      //mIsDirty = false;
    }

    if (mSimpleTimer.check()) { // check if the metro has passed its interval .
      //neutral();
      delay(1);
      FastLED.show();
      //mIsDirty = false;
    }
  }
}

void Fastled_led::command(byte command)
{
  Logger::Instance().write(LogLevel::DEBUG, "recievecommand: " + (String)(command));
  Commands comm = static_cast<Commands>(command);
  if (comm == Commands::VEHICLE_ISDEAD)
  {
      reset();
  }
  else if (comm == Commands::VEHICLE_ISALIVE)
  {
    reset();
    neutral();
  }
  else if (comm == Commands::DRIVE_NEUTRAL)
  {
    neutral();
  }
  else if (comm == Commands::DRIVE_POWER)
  {
    acc(true);
    brake(false);
  }
  else if (comm == Commands::DRIVE_BRAKE)
  {
    acc(false);
    brake(true);
  }
  else if (comm == Commands::TURN_LEFT)
  {
    left(true);
    right(false);
  }
  else if (comm == Commands::TURN_RIGHT)
  {
    left(false);
    right(true);
  }

}

void Fastled_led::reset()
{
  for(int i=0;i<FASTLED_PIXELS;i++){
     leds[i] = CRGB(0,0,0);
     mIsDirty = true;
   }
}

void Fastled_led::neutral()
{
  for(int i=0;i<FASTLED_PIXELS;i++){
     // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
     if (i % 3 == 0)
      {
         leds[i] = CRGB(50,50,0);
         mIsDirty = true;
      }
   }
}

void Fastled_led::acc(bool enable)
{
  if (enable)
  {
    leds[0] = CRGB(0,100,0);
    leds[1] = CRGB(0,15,0);
    leds[FASTLED_PIXELS-1] = CRGB(0,15,0);
  }
  else
  {
    leds[0] = CRGB(0,0,0);
    leds[1] = CRGB(0,0,0);
    leds[FASTLED_PIXELS-1] = CRGB(0,0,0);
  }
  mIsDirty = true;
}

void Fastled_led::brake(bool enable)
{
  if (enable)
  {
      leds[0+(FASTLED_PIXELS/2)-1] = CRGB(15,0,0);
      leds[1+(FASTLED_PIXELS/2)-1] = CRGB(100,0,0);
      leds[FASTLED_PIXELS-(FASTLED_PIXELS/2)+1] = CRGB(15,0,0);
  }
  else
  {
    leds[0+(FASTLED_PIXELS/2)-1] = CRGB(0,0,0);
    leds[1+(FASTLED_PIXELS/2)-1] = CRGB(0,0,0);
    leds[FASTLED_PIXELS-(FASTLED_PIXELS/2)+1] = CRGB(0,0,0);
  }
  mIsDirty = true;
}

void Fastled_led::left(bool enable)
{
  if (enable)
  {
    leds[2+(FASTLED_PIXELS/2)] = CRGB(15,15,0);
    leds[3+(FASTLED_PIXELS/2)] = CRGB(50,50,0);
    leds[4+(FASTLED_PIXELS/2)] = CRGB(15,15,0);
  }
  else
  {
    leds[2+(FASTLED_PIXELS/2)] = CRGB(0,0,0);
    leds[3+(FASTLED_PIXELS/2)] = CRGB(0,0,0);
    leds[4+(FASTLED_PIXELS/2)] = CRGB(0,0,0);
  }
  mIsDirty = true;
}

void Fastled_led::right(bool enable)
{
  if (enable)
  {
    leds[2] = CRGB(15,15,0);
    leds[3] = CRGB(50,50,0);
    leds[4] = CRGB(15,15,0);
  }
  else
  {
    leds[2] = CRGB(0,0,0);
    leds[3] = CRGB(0,0,0);
    leds[4] = CRGB(0,0,0);
  }
  mIsDirty = true;
}

void Fastled_led::onDisable()
{
  reset();
  FastLED.show();
}
