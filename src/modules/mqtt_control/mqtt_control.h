#ifndef MQTT_CONTROL_H
#define MQTT_CONTROL_H

#include <Arduino.h>
#include <WebSocketsServer.h>
#include "../../configuration/configurator.h"
#include "./mqtt_control_config.h"
#include "../../utility/simpletimer.h"
#include "../base/base.hpp"
#include "../../interfaces/interfaces.hpp"

class Mqtt_control : public virtual Control_module
{
private:
  IFMV * mFMV;
  SimpleTimer mSimpleTimer;
  Mqtt_control_config * mCfg = nullptr;
  IConnection * pIConnection = nullptr;
  void addReciever();
protected:
  void onDisable();
  void onEvent(byte eventId, bool always = false)
  {
    if (always || isActive())
    {
      //Logger::Instance().write(LogLevel::INFO, getModuleName(), FPSTR(" onEvent moduleEvent eventId: ") + String(eventId));
      mFMV -> moduleEvent(this, eventId);
    }
  }
  void handleCommand(byte command, byte value);
public:
  Mqtt_control(byte id, IFMV * fmv, Mqtt_control_config * cfg = nullptr) : Control_module(id, Modules::MQTT_CONTROL)  {
    mFMV = fmv;
    if (cfg == nullptr)
      mCfg = static_cast<Mqtt_control_config*>(Configurator::Instance().getConfig(id, Configurations::MQTT_CONTROL_CONFIG));
    else
      mCfg = cfg;
    setConfig();
  }

  void setConfig()
  {
    mSimpleTimer.setName("Mqtt_control");
    mSimpleTimer.setInterval(10);
    setClientTimeout("Mqtt_control", 10000);
    setEnabled(mCfg -> enabled);
  }

  void setup();
  void loop();
  void command(byte command);

  String getModuleName()
  {
    return FPSTR("MQTT_CONTROL");
  }
};

#endif
