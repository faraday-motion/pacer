#ifndef FMV_H
#define FMV_H

#include <Arduino.h>
#include <vector>
#include "./utility/simpletimer.h"
#include "./macros.h"
#include "./log/logger.h"
#include "./enums/enums.hpp"
#include "./modules/base/base.hpp"
#include "./configuration/wheel.h"
#include "./modules/modulelist.h"
#include "./sensors/sensorlist.h"
#include "./utility/spiffs_config.h"
#include "./utility/spiffs_storage.h"
#include "./configuration/base/default_eventrulesbase.h"


class FMV {
private:
  void getFactoryInstances(std::vector<Configbase*> mConfigArray);
  const String mVersion = "1.0";
  std::vector<Wheel*> wheelArray;
  bool mIsSetup = false;
  ModuleList * mModules = new ModuleList();
  SensorList * mSensors = new SensorList();
  Default_eventrulesbase* mEventRules = nullptr;
  Spiffs_config * pSpiffs_config;
  Spiffs_storage * pSpiffs_storage;
  SimpleTimer * pSimpleTimerPingPong = new SimpleTimer(1000);
public:
  FMV() {

  };
  virtual void setup();
  virtual void loop();
  void moduleEvent(Modulebase* sender, byte eventId);

  std::vector<Wheel*> getWheelValues() const
  {
    return wheelArray;
  }

  ModuleList &modules()
  {
    return *mModules;
  }

  SensorList &sensors()
  {
    return *mSensors;
  }

  String getVersion()
  {
    return mVersion;
  }
};
#endif
