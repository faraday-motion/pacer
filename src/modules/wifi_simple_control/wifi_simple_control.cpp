#include <Arduino.h>
#include "./wifi_simple_control.h"
#include "../../configuration/default/configuration.h"
#include "../../logs/logger.h"

void Wifi_simple_control::setup() {
  if (mIsSetup == false)
  {
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up Wifi_simple_control"));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    onEvent(Events::CONFIGURE, true);
    pServer = new WiFiServer(8899);
    pServer -> begin();
    pServer -> setNoDelay(true);
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up Wifi_simple_control"));
    mIsSetup = true;
  }
}

void Wifi_simple_control::loop()
{
  if (enabled())
  {
    if (mSimpleTimerConnection.check())
    {
      handleClientConnections();
    }
    if (mSimpleTimerInput.check())
    {
      Logger::Instance().write(LogLevel::DEBUG, FPSTR("Wifi_simple_control::loop"));
      readInput();
      if (isActive())
      {
        mFMV -> sensors().add("active", id());
        Logger::Instance().write(LogLevel::DEBUG, FPSTR("Wifi_simple_control: "), String(mOutputControl.getPower()) + " " + String(mOutputControl.getBrake()));
      }
      clientTimeoutCheck();
    }
  }
}

void Wifi_simple_control::handleClientConnections() {
  if (pServer->hasClient())
  {
    mClient = pServer->available();
    if (mClient && mClient.connected())
      mClient.setNoDelay(true);
    else
      mClient.stop();
  }
}

void Wifi_simple_control::readInput()
{
   uint8_t i;
   // Check clients for data
   if (mClient && mClient.connected()) {
     unsigned int streamLength = mClient.available();
     if (streamLength > 0) {
       // size_t len
       byte packetLength = 11;
       byte packetCount  = 0;
       byte m[packetLength];

       unsigned int packages = int(float(streamLength / packetLength));
       for (uint8_t ii = 0; ii < (packages - 1) * packetLength; ii++)
       {
         mClient.read();
       }
       for (uint8_t ii = 0; ii < packetLength; ii++)
       {
         m[packetCount++] = mClient.read();
       }
       yield();
       if (Tools::validateChecksum(m, packetCount))
       {
          yield();
          processInput(m[4]);
          yield();
       }
       else
       {
         //Checksum invalid
         Logger::Instance().write(LogLevel::ERROR, FPSTR("Wifi_simple_control: Checksum invalid"));
       }
     }
   }
}

void Wifi_simple_control::processInput(byte input)
{
  byte defaultInputNeutral = 50;
  byte defaultInputMinBrake = 48;
  byte defaultInputMaxBrake = 0;
  byte defaultInputMinAcceleration = 52;
  byte defaultInputMaxAcceleration = 100;
  input = constrain(input, defaultInputMaxBrake, defaultInputMaxAcceleration);
  if (input > defaultInputMinAcceleration)
  {
    //Accelerate
    mBack = 0;
    mForward = map(input, defaultInputMinAcceleration, defaultInputMaxAcceleration, 0, 100);
    mOutputControl.setPower(mForward);
    onEvent(Events::DRIVE_POWER);
    setHasClient(true);
  }
  else if (input < defaultInputMinBrake)
  {
    //Brake
    mForward = 0;
    mBack = map(input, defaultInputMinBrake, defaultInputMaxBrake, 0, 100);
    mOutputControl.setBrake(mBack);
    onEvent(Events::DRIVE_BRAKE);
    setHasClient(true);
  }
  else
  {
    //Neutral
    mForward = 0;
    mBack = 0;
    mOutputControl.resetPower();
    onEvent(Events::DRIVE_NEUTRAL);
    setHasClient(true);
  }
}

void Wifi_simple_control::command(byte command)
{

}

void Wifi_simple_control::onEnable()
{
   onEvent(Events::MODULE_ENABLED);
}


void Wifi_simple_control::onDisable()
{
   mForward = 0;
   mBack = 0;
   mOutputControl.resetPower();
   onEvent(Events::CONTROL_NONE);
   onEvent(Events::MODULE_DISABLED);
}
