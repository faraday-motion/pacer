#ifndef VESC_CONTROLLER_H
#define VESC_CONTROLLER_H

#include <vector>
#include <Arduino.h>
#include "../../configuration/configurator.h"
#include "./vesc_controller_config.h"
#include "./vesc_controller_wheel_decorator.h"
#include "../base/input_controller.h"
#include "../../fmv.h"
#include "../../sensors/base/sensorbase.h"
#include "datatypes.h"

static bool sVescDefaultSerial = false;
static HardwareSerial * sVescSerial = nullptr;

class Vesc_controller : virtual public Input_controller
{
private:
  FMV *mFMV;
  SimpleTimer mSimpleTimer;
  Vesc_controller_config* mCfg = nullptr;
  void serialInit();
  void serialRead();
  void setCurrent();
  void setRpm(int rpm);
  void getValues();
  void createTimerTask();
  byte mMaxPowerCurrent = 0;
  byte mMaxBrakeCurrent = 0;
  int mMaxRpm = 0;
  hw_timer_t* timerVesc = nullptr;
  std::vector<Vesc_controller_wheel_decorator*> wheelDecorators;
protected:
  void onDisable();
  void onEvent(byte eventId)
  {
    mFMV -> moduleEvent(this, eventId);
  }
public:
  Vesc_controller(byte id, FMV *fmv, Vesc_controller_config* cfg = nullptr) : Input_controller(id, Modules::VESC_CONTROLLER)  {
    mFMV = fmv;
    if (cfg == nullptr)
      mCfg = static_cast<Vesc_controller_config*>(Configurator::Instance().createConfig(id, Configurations::VESC_CONTROLLER_CONFIG));
    else
      mCfg = cfg;
    setConfig();
  }

  enum Commands : byte {
    CURRENT_MODE,
    RPM_MODE
  };

  enum Events : byte {

  };

  void setConfig()
  {
    mSimpleTimer.setName("Vesc_controller");
    mSimpleTimer.setInterval(25, 50);
    sVescDefaultSerial = mCfg -> defaultSerial;
    mMaxPowerCurrent = mCfg -> maxPowerCurrent;
    mMaxBrakeCurrent = mCfg -> maxBrakeCurrent;
    mMaxRpm = mCfg -> maxRpm;
    setEnabled(mCfg -> enabled);
  }

  void setup();
  void loop();
  void command(byte command);
  static void timerRun();
};

#endif
