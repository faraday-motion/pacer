//#include <string>
#include <Arduino.h>
#include "./serial_control.h"
#include "../../configuration/default/configuration.h"
#include "../../configuration/configurator.h"
#include "../../enums/enums.hpp"

//TODO REMOVE
#include "../wifi_connection/Wifi_connection.h"

using namespace std;

void Serial_control::setup() {
  if (mIsSetup == false)
  {
    Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up Serial_control"));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    onEvent(Events::CONFIGURE, true);
    Configurator::Instance().initializeSerial();
    Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up Serial_control"));
    mIsSetup = true;
  }
}

void Serial_control::loop()
{
  if (enabled())
  {
    Logger::Instance().write(LogLevel::DEBUG, FPSTR("Pwm_steering::loop"));
    byte command = 0;
    byte value = 0;
    if (readSerial(command, value))
    {
      handleCommand(command, value);
      if (command == 99)
        mFMV -> modules().command(Modules::WIFI_CONNECTION, (byte)Wifi_connection::Commands::CONNECTION_WIFI_STA);
    }

    if (isActive())
    {
      mFMV -> sensors().add("Active", id());
      Logger::Instance().write(LogLevel::DEBUG, FPSTR("Serial_control::Power "), String(mOutputControl.getPower()) + " " + String(mOutputControl.getBrake()));
      Logger::Instance().write(LogLevel::DEBUG, FPSTR("Serial_control::Turning "), String(mOutputControl.getLeft()) + " " + String(mOutputControl.getRight()));
    }
    clientTimeoutCheck();
  }
}

bool Serial_control::readSerial(byte &command, byte &value)
{
  if (Serial.available() > 0)
  {
    char buffer[Serial.available()];
    byte index = 0;
    int len = Serial.available();
    while (len--) {
      buffer[index++] = Serial.read();
    }
    string str = buffer;
    string::size_type pos = str.find(':');
    string com = str.substr(0, pos);
    string val = str.substr(pos+1);
    command = std::atoi(com.c_str());
    value = std::atoi(val.c_str());
    return true;
  }
  return false;
}

void Serial_control::command(byte command)
{

}

void Serial_control::onDisable()
{
   mOutputControl.reset();
   onEvent(Events::CONTROL_NONE);
}
