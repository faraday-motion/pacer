#ifndef SERIAL_LOG_H
#define SERIAL_LOG_H

#include <Arduino.h>
#include "../../configuration/configurator.h"
#include "./serial_log_config.h"
#include "../base/base.hpp"

class Serial_log : public virtual Log_module
{
private:
  Serial_log_config * mCfg = nullptr;
  IFMV * mFMV = nullptr;
protected:
  void onEvent(byte eventId, bool always = false)
  {
    mFMV -> moduleEvent(this, eventId);
  }
public:
    Serial_log(byte id, IFMV * fmv = nullptr, Serial_log_config * mfg = nullptr) : Log_module(id, Modules::SERIAL_LOG){
      mFMV = fmv;
      Configurator::Instance().initializeSerial();
      if (mfg == nullptr)
        mCfg = static_cast<Serial_log_config*>(Configurator::Instance().getConfig(id, Configurations::SERIAL_LOG_CONFIG));
      else
        mCfg = mfg;
      setConfig();
    };

    enum Events : byte {
      CONFIGURE
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
        mIsSetup = true;
        Logger::Instance().write(LogLevel::INFO, FPSTR("Setting up "), getModuleName());
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        onEvent(Events::CONFIGURE);

        Logger::Instance().addLog(this);
        Logger::Instance().write(LogLevel::INFO, FPSTR("Finished setting up "), getModuleName());
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
