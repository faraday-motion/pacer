#ifndef VESC_CONTROLLER_H
#define VESC_CONTROLLER_H

#include <vector>
#include <functional>
#include <Arduino.h>
#include "../../configuration/configurator.h"
#include "./vesc_controller_config.h"
#include "./vesc_controller_wheel_decorator.h"
#include "../base/power_module.h"
#include "../../fmv.h"
#include "../../sensors/base/sensorbase.h"
#include "./vesc_datatypes.h"
#include "./vesc_interface.h"
#include "./ivesc_interface.h"

//When the vesc_interface is not static, it gives problems
static Vesc_interface * pVescInterface;

class Vesc_controller : public Power_module, public IVesc_interface
{
private:
  FMV * mFMV;
  SimpleTimer mSimpleTimer;
  Vesc_controller_config * mCfg = nullptr;


  void setCurrent();
  void setRpm(int rpm);
  void getValues();
  void createTimerTask();
  byte mMaxPowerCurrent = 0;
  byte mMaxBrakeCurrent = 0;
  int mMaxRpm = 0;
  byte mVescArrayIndex = 0;
  std::vector<Vesc_controller_wheel_decorator*> wheelDecorators;
  mc_values mMotorValues;
  bool mVescDefaultSerial;
  void setValues(mc_values * val);
  void print(char *str);
  void setFirmware(int major, int minor);
  void setRotorPos(float pos);
  void setMotorConfiguration(mc_configuration *conf);
  void setAppConfiguration(app_configuration *conf);
  void setDetect(float cycle_int_limit, float coupling_k, const signed char *hall_table, signed char hall_res);
  void setPPM(float val, float ms);
  void setADC(float val, float voltage);
  void setChuk(float val);
  void motorConfigurationRecieved();
  void appConfigurationRecieved();
protected:
  void onDisable();
  void onEvent(byte eventId)
  {
    mFMV -> moduleEvent(this, eventId);
  }
public:

  Vesc_controller(byte id, FMV *fmv, Vesc_controller_config* cfg = nullptr) : Power_module(id, Modules::VESC_CONTROLLER) {
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
    mVescDefaultSerial = mCfg -> defaultSerial;
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
