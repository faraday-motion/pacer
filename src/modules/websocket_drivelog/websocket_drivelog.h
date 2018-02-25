#ifndef WEBSOCKET_DRIVELOG_H
#define WEBSOCKET_DRIVELOG_H
#include <Arduino.h>
#include "../base/base.hpp"
#include "../../configuration/configurator.h"
#include "./websocket_drivelog_config.h"
#include "../../utility/simpletimer.h"

class Websocket_drivelog : virtual public Modulebase
{
  private:
    FMV *mFMV;
    void writeSensorValues();
    SimpleTimer mSimpleTimer;
    Websocket_drivelog_config * mCfg = nullptr;
    IConnection * pIConnection = nullptr;
    void getWebsocketConnection();
  protected:
  public:
    Websocket_drivelog(byte id, FMV *fmv, Websocket_drivelog_config* cfg = nullptr) : Modulebase(id, Modules::WEBSOCKET_DRIVELOG)  {
      mFMV = fmv;
      if (cfg == nullptr)
        mCfg = static_cast<Websocket_drivelog_config*>(Configurator::Instance().createConfig(id, Configurations::WEBSOCKET_DRIVELOG_CONFIG));
      else
        mCfg = cfg;
      setConfig();
    }

    void setConfig()
    {
      mSimpleTimer.setName("Websocket_drivelog");
      mSimpleTimer.setInterval(mCfg -> interval, mCfg -> critical);
      setEnabled(mCfg -> enabled);
    }

  void setup();
  void loop();
  void command(byte command);

  String getModuleName()
  {
    return FPSTR("WEBSOCKET_DRIVELOG");
  }
};

#endif
