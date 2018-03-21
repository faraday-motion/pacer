#ifndef ANALOG_SENSOR_H
#define ANALOG_SENSOR_H

#include <Arduino.h>
#include <functional>
#include "../../configuration/configurator.h"
#include "./analog_sensor_config.h"
#include "../base/modulebase.h"
#include "../../fmv.h"
#include "../../utility/simpletimer.h"
#include "../../interfaces/interfaces.hpp"

class Analog_sensor : public virtual Modulebase
{
private:
  IFMV * mFMV = nullptr;
  Analog_sensor_config * mCfg = nullptr;
  SimpleTimer mSimpleTimer;
protected:
  void onEvent(byte eventId, bool always = false)
  {
    mFMV -> moduleEvent(this, eventId);
  }
public:
    Analog_sensor(byte id, IFMV * fmv, Analog_sensor_config * cfg = nullptr) : Modulebase(id, Modules::ANALOG_SENSOR){
      mFMV = fmv;
      if(cfg == nullptr)
        mCfg = static_cast<Analog_sensor_config*>(Configurator::Instance().createConfig(id, Configurations::ANALOG_SENSOR_CONFIG));
      else
        mCfg = cfg;
      setConfig();
    }

    void setConfig()
    {
      mSimpleTimer.setName("Analog_sensor");
      mSimpleTimer.setInterval(mCfg -> interval, mCfg -> critical);
      setEnabled(mCfg -> enabled);
    }

    void setup();
    void loop();
    int value();
    void readAnalogPin();

    void command(byte command);

    String getModuleName()
    {
      return FPSTR("ANALOG_SENSOR");
    }
};

#endif
