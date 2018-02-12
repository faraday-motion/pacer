#ifndef FMV_H
#define FMV_H

#include <Arduino.h>
#include <vector>
#include "./utility/simpletimer.h"
#include "./macros.h"
#include "./logs/logger.h"
#include "./sensors/base/sensorbase.h"
#include "./sensors/sensor_value.h"
#include "./enums/modules.h"
#include "./enums/externalcommands.h"
#include "./modules/base/modulebase.h"
#include "./configuration/wheel.h"
#include "./modules/modulelist.h"
#include "./sensors/sensorlist.h"
#include "./utility/spiffs_config.h"
#include "./utility/spiffs_storage.h"
#include "./configuration/base/default_eventrulesbase.h"

class FMV {
private:
  const String mVersion = "1.0";
  std::vector<Wheel*> wheelArray;
  std::vector<Sensorbase*> sensorArray;
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
  void getFactoryInstances(std::vector<Configbase*> mConfigArray);
  void moduleEvent(Modulebase* sender, byte eventId);
  Sensorbase* getSensor(byte id);

  std::vector<Wheel*> getWheelValues() const
  {
    return wheelArray;
  }

  ModuleList &modules() const
  {
    return *mModules;
  }

  SensorList &sensors() const
  {
    return *mSensors;
  }

  String getVersion()
  {
    return mVersion;
  }
};
#endif
