#ifndef SERIAL_LOG_H
#define SERIAL_LOG_H

#include <Arduino.h>
#include "../base/logbase.h"
#include "../../configuration/configurator.h"
#include "./serial_log_config.h"
#include "../../enums/loglevel.h"
#include "../../fmv.h"

class Serial_log : public Logbase
{
private:
    Serial_log_config* mCfg = nullptr;
    FMV * mFMV = nullptr;
protected:
public:
    Serial_log(byte id, FMV *fmv = nullptr, Serial_log_config* mfg = nullptr) : Logbase(id){
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
};

#endif
