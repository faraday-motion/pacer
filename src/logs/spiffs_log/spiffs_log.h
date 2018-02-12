#ifndef SPIFFS_LOG_H
#define SPIFFS_LOG_H

#include <Arduino.h>
#include "../base/logbase.h"
#include "../../utility/tools.h"
#include "../../utility/spiffs_storage.h"
#include "../../configuration/configurator.h"
#include "./spiffs_log_config.h"
#include "../../enums/loglevel.h"
#include "../../fmv.h"

class Spiffs_log : public Logbase
{
private:
      Spiffs_log_config* mCfg = nullptr;
      String mLogFile;
      Spiffs_storage mSpiffs_storage;
      FMV * mFMV = nullptr;
protected:
public:
    Spiffs_log(byte id, FMV *fmv = nullptr, Spiffs_log_config* mfg = nullptr) : Logbase(id){
      mFMV = fmv;
      Configurator::Instance().initializeSpiff();
      if (mfg == nullptr)
        mCfg = static_cast<Spiffs_log_config*>(Configurator::Instance().createConfig(id, Configurations::SPIFFS_LOG_CONFIG));
      else
        mCfg = mfg;
      setConfig();
    };

    void setConfig()
    {
      setLevel(mCfg -> logLevel);
      mLogFile = mCfg -> logFile;
      setEnabled(mCfg -> enabled);
    }

    void write(LogLevel level, String message)
    {
      if (enabled() && level >= getLevel())
      {
        mSpiffs_storage.append(mLogFile, message + "\n");
      }
    }
};

#endif
