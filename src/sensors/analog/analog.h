#ifndef ANALOG_H
#define ANALOG_H

#include <Arduino.h>
#include <functional>
#include "../../configuration/configurator.h"
#include "../../utility/simpletimer.h"
#include "../base/sensorbase.h"

class Analog : virtual public Sensorbase {
private:
  byte mPin = -1;
  Analog_config* mCfg = nullptr;
protected:
public:
    Analog(byte id, Analog_config* cfg = nullptr) : Sensorbase(id, Sensors::SENSOR_ANALOG){
      if(cfg == nullptr)
        mCfg = static_cast<Analog_config*>(Configurator::Instance().createConfig(id, Configurations::ANALOG_CONFIG));
      else
        mCfg = cfg;
      setConfig();
    }

    void setConfig()
    {
      mPin = mCfg -> pin;
      mSimpleTimer.setName("Analog");
      mSimpleTimer.setInterval(mCfg -> interval, mCfg -> critical);
    }

    void setup();
    void loop();
    int value();
    void readAnalogPin();
};

#endif
