#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H
#include <Arduino.h>
#include <vector>

#include "SPIFFS.h"
#include "./default/configuration.h"
#include "./base/configbase.h"

#include "../enums/modules.h"
#include "../enums/configurations.h"
#include "../utility/tools.h"
#include "../log/logger.h"
#include "../modules/configlist.h"

class Configurator{
private:
  bool mIsSerialInitialized = false;
  bool mIsSpiffInitialized = false;
  Configlist mConfigs;
  struct tm mTimeInfo;
protected:
  Configurator(){}; // Constructor is protected
  Configurator(Configurator const&){};// copy constructor is protected
  Configurator& operator=(Configurator const&){}; //assignment operator is protected.
public:
  static Configurator &Instance()
  {
      // This line only runs once, thus creating the only instance in existence
      static Configurator *instance = new Configurator;
      // dereferencing the variable here, saves the caller from having to use
      // the arrow operator, and removes temptation to try and delete the
      // returned instance.
      return *instance; // always returns the same instance
  }

  tm* getTimeInfo()
  {
    return &mTimeInfo;
  }

  bool addConfig(Configbase* config, bool save = false)
  {
    return mConfigs.add(config, save);
  }

  Configbase* createConfig(byte id, int configuration)
  {
    return mConfigs.create(id, configuration);
  }

  Configlist configs() const
  {
    return mConfigs;
  }

  void initializeSerial()
  {
    if (!mIsSerialInitialized)
    {
      Logger::Instance().write(LogLevel::INFO, FPSTR("Initializing Serial"));
      Serial.begin(BAUDRATE);
      while (!Serial);
      Logger::Instance().write(LogLevel::INFO, FPSTR("Finished initializing Serial"));
      mIsSerialInitialized = true;
    }
  }

  void initializeSpiff()
  {
    if (!mIsSpiffInitialized)
    {
      Logger::Instance().write(LogLevel::INFO, FPSTR("Initializing Spiff"));
      //The memory in the chip is first available in a fraction of time after restart
      delay(100);
      if(!SPIFFS.begin(FORMATONFAILURE)){
        Logger::Instance().write(LogLevel::WARNING, FPSTR("Spiff mount failed"));
      }
      Logger::Instance().write(LogLevel::INFO, FPSTR("Finished initializing Spiff"));
      mIsSpiffInitialized = true;
    }
  }

  void initializeAnalog()
  {
    Logger::Instance().write(LogLevel::INFO, FPSTR("Initializing Analog"));
    analogReadResolution(10);
    analogSetAttenuation(ADC_6db);
  }


};

#endif
