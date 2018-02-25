#ifndef SERIAL_LOG_H
#define SERIAL_LOG_H

#include <Arduino.h>
#include "../../configuration/configurator.h"
#include "./serial_log_config.h"
#include "../base/base.hpp"

class Serial_log : virtual public Log_module
{
private:
    Serial_log_config* mCfg = nullptr;
    FMV * mFMV = nullptr;
protected:
  void onEvent(byte eventId)
  {
    mFMV -> moduleEvent(this, eventId);
  }
public:
    Serial_log(byte id, FMV * fmv = nullptr, Serial_log_config * mfg = nullptr) : Log_module(id, Modules::SERIAL_LOG){
      mFMV = fmv;
      Configurator::Instance().initializeSerial();
      if (mfg == nullptr)
        mCfg = static_cast<Serial_log_config*>(Configurator::Instance().createConfig(id, Configurations::SERIAL_LOG_CONFIG));
      else
        mCfg = mfg;
      setConfig();
    };

    void setConfig()
    {
      setLevel(mCfg -> logLevel);
      setEnabled(mCfg -> enabled);
    }

    void write(LogLevel level, String message)
    {
      if (enabled() && level >= getLevel())
      {
        Serial.println(message);
      }
    }

    void setup()
    {
      if (mIsSetup == false)
      {
        Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up "), getModuleName());
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        Logger::Instance().addLog(this);
        Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up "), getModuleName());
        mIsSetup = true;
      }
    }

    void loop()
    {
    }

    void command(byte command)
    {
    }

    String getModuleName()
    {
      return FPSTR("SERIAL_LOG");
    }


};

#endif
