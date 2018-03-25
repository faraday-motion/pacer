#ifndef FMV_H
#define FMV_H

#include <Arduino.h>
#include <vector>
#include <Preferences.h>
#include "./utility/simpletimer.h"
#include "./macros.h"
#include "./log/logger.h"
#include "./enums/enums.hpp"
#include "./modules/base/base.hpp"
#include "./modules/modulelist.h"
#include "./sensors/sensorlist.h"
#include "./utility/spiffs_config.h"
#include "./utility/spiffs_storage.h"
#include "./configuration/base/default_eventrulesbase.h"
#include "./interfaces/interfaces.hpp"

class FMV : public virtual IFMV {
private:
  void getFactoryInstances(std::vector<Configbase*> mConfigArray);
  const String mVersion = "1.0";
  std::vector<IWheel*> wheelArray;
  bool mIsSetup = false;
  IModuleList * mModules = new ModuleList();
  ISensorList * mSensors = new SensorList();
  Default_eventrulesbase * mEventRules = nullptr;
  Spiffs_config * pSpiffs_config;
  Spiffs_storage * pSpiffs_storage;
  SimpleTimer * pSimpleTimerPingPong = new SimpleTimer(1000);
  Preferences preferences;
  void incrementRestarts();
  void addEnabledCommands();
public:
  FMV() {

  };
  void setup();
  void loop();
  void moduleEvent(IModule * sender, byte eventId);

  std::vector<IWheel*> getWheelValues() const
  {
    return wheelArray;
  }

  IModuleList &modules()
  {
    return * mModules;
  }

  Default_eventrulesbase &eventRules()
  {
    return * mEventRules;
  }

  void setEventRules(Default_eventrulesbase * rules)
  {
    mEventRules = rules;
    mEventRules -> setFMV(this);
  }

  ISensorList &sensors()
  {
    return * mSensors;
  }

  String getVersion()
  {
    return mVersion;
  }
};
#endif
