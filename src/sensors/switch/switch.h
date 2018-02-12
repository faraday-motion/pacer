#ifndef SWITCH_H
#define SWITCH_H

#include <Arduino.h>
#include <functional>
#include "../../configuration/configurator.h"
#include "../../utility/simpletimer.h"
#include "../base/sensorbase.h"
#include "../../enums/enums.hpp"

class Switch : virtual public Sensorbase {
private:
  byte mPin = 0;
  bool mInvert = false;
  Switch_config* mCfg = nullptr;
protected:
public:
    Switch(byte id, Switch_config* cfg = nullptr) : Sensorbase(id, Sensors::SENSOR_SWITCH)
    {
      if(cfg == nullptr)
        mCfg = static_cast<Switch_config*>(Configurator::Instance().createConfig(id, Configurations::SWITCH_CONFIG));
      else
        mCfg = cfg;
      setConfig();
    }

    void setConfig()
    {
      mPin = mCfg -> pin;
      mInvert = mCfg -> invert;
      mSimpleTimer.setName("Switch");
      mSimpleTimer.setInterval(mCfg -> interval, mCfg -> critical);
    }

    void setup();
    void loop();
    int value();
    void readSwitchPin();
};

#endif
