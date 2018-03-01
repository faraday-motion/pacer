#ifndef ANALOG_SENSOR_H
#define ANALOG_SENSOR_H

#include <Arduino.h>
#include <functional>
#include "../../configuration/configurator.h"
#include "./analog_sensor_config.h"
#include "../base/modulebase.h"
#include "../../fmv.h"
#include "../../utility/simpletimer.h"

class Analog_sensor : virtual public Modulebase
{
private:
  FMV * mFMV = nullptr;
  byte mPin = -1;
  String mSensorName = "";
  Analog_sensor_config * mCfg = nullptr;
  SimpleTimer mSimpleTimer;
protected:
  void onEvent(byte eventId, bool always = false)
  {
    mFMV -> moduleEvent(this, eventId);
  }
public:
    Analog_sensor(byte id, FMV *fmv, Analog_sensor_config* cfg = nullptr) : Modulebase(id, Modules::ANALOG_SENSOR){
      mFMV = fmv;
      if(cfg == nullptr)
        mCfg = static_cast<Analog_sensor_config*>(Configurator::Instance().createConfig(id, Configurations::ANALOG_SENSOR_CONFIG));
      else
        mCfg = cfg;
      setConfig();
    }

    void setConfig()
    {
      mPin = mCfg -> pin;
      mSimpleTimer.setName("Analog_sensor");
      mSimpleTimer.setInterval(mCfg -> interval, mCfg -> critical);
      mSensorName = mCfg -> sensorName;
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
