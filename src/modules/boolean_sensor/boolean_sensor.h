#ifndef BOOLEAN_SENSOR_H
#define BOOLEAN_SENSOR_H

#include <Arduino.h>
#include <functional>
#include "../../configuration/configurator.h"
#include "./boolean_sensor_config.h"
#include "../base/modulebase.h"
#include "../../fmv.h"
#include "../../utility/simpletimer.h"

class Boolean_sensor : public virtual Modulebase {
private:
  IFMV * mFMV = nullptr;
  byte mPin = 0;
  bool mInvert = false;
  Boolean_sensor_config * mCfg = nullptr;
  String mSensorName = "";
  SimpleTimer mSimpleTimer;
protected:
  void onEvent(byte eventId, bool always = false)
  {
    mFMV -> moduleEvent(this, eventId);
  }
public:
    Boolean_sensor(byte id, IFMV * fmv, Boolean_sensor_config * cfg = nullptr) : Modulebase(id, Modules::BOOLEAN_SENSOR)
    {
      mFMV = fmv;
      if(cfg == nullptr)
        mCfg = static_cast<Boolean_sensor_config*>(Configurator::Instance().createConfig(id, Configurations::BOOLEAN_SENSOR_CONFIG));
      else
        mCfg = cfg;
      setConfig();
    }

    void setConfig()
    {
      mPin = mCfg -> pin;
      mInvert = mCfg -> invert;
      mSimpleTimer.setName("Boolean_sensor");
      mSimpleTimer.setInterval(mCfg -> interval, mCfg -> critical);
      mSensorName = mCfg -> sensorName;
      setEnabled(mCfg -> enabled);
    }

    void setup();
    void loop();
    void command(byte command);
    int value();
    void readBooleanPin();

    String getModuleName()
    {
      return FPSTR("BOOLEAN_SENSOR");
    }
};

#endif
