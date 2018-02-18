#ifndef SENSORFACTORY_H
#define SENSORFACTORY_H
#include <Arduino.h>
#include "../configuration/default/configuration.h"
#include "../configuration/base/configbase.h"
#include "../configuration/base/default_configbase.h"
#include "../enums/configurations.h"
#include "../fmv.h"

#include "../sensors/switch/switch.h"
#include "../sensors/analog/analog.h"
#include "./sensors/sensor_value.h"

class Sensorfactory{
public:

  static Sensorbase* getSensorInstance(byte id, int configuration, FMV *fmv)
  {
    Sensorbase* sen = nullptr;
      switch(configuration) {
      case Configurations::SWITCH_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getSensorInstance SWITCH_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        sen = new Switch(id);
        break;
      case Configurations::ANALOG_CONFIG :
        Logger::Instance().write(LogLevel::INFO, FPSTR("getSensorInstance ANALOG_CONFIG"));
        Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
        sen = new Analog(id);
        break;
      }
      return sen;
  }
};

#endif
