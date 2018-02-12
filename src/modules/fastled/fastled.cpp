#include <Arduino.h>
#define FASTLED_FORCE_SOFTWARE_SPI
#include <FastLED.h>
//FASTLED_USING_NAMESPACE;
#include "../../utility/simpletimer.h"
#include "./fastled.h"
#include "../../configuration/default/configuration.h"

CRGB leds[FASTLED_PIXELS];

void Fastled::setup()
{
  if (mIsSetup == false)
  {
    Logger::Instance().write(LogLevel::INFO, "Setting up Fastled");
    pinMode(PIN_FASTLED, OUTPUT);
    // set master brightness control
    FastLED.addLeds<NEOPIXEL, PIN_FASTLED>(leds, FASTLED_PIXELS);
    FastLED.setBrightness(100);
    reset();
    neutral();
    Logger::Instance().write(LogLevel::INFO, "Finished setting up Fastled");
    mIsSetup = true;
  }
}

void Fastled::loop()
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

void Fastled::command(byte command)
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

void Fastled::reset()
{
  for(int i=0;i<FASTLED_PIXELS;i++){
     leds[i] = CRGB(0,0,0);
     mIsDirty = true;
   }
}

void Fastled::neutral()
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

void Fastled::acc(bool enable)
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

void Fastled::brake(bool enable)
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

void Fastled::left(bool enable)
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

void Fastled::right(bool enable)
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

void Fastled::onDisable()
{
  reset();
  FastLED.show();
}
