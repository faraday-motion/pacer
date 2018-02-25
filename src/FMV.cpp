#include <vector>
#include <Arduino.h>
#include <rom/rtc.h>

#include "./modules/base/base.hpp"
#include "./log/logger.h"
#include "./configuration/wheel.h"
#include "./configuration/base/configbase.h"
#include "./configuration/default/configuration.h"
#include "./configuration/configurator.h"
#include "./configuration/custom/custom_config.h"
#include "./configuration/custom/custom_eventrules.h"

#include "./factory/modulefactory.h"

#include "./enums/modules.h"

void FMV::setup() {
  if (mIsSetup == false)
  {
    Logger::Instance().write(LogLevel::INFO, FPSTR("************* Setting up Faraday Motion Pacer v"),  String(mVersion));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Reset Reason Core 0: "), Tools::resetReason(rtc_get_reset_reason(0)));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Reset Reason Core 1: "), Tools::resetReason(rtc_get_reset_reason(1)));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Running Core: "), String(xPortGetCoreID()));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));

    Configurator::Instance().initializeAnalog();
    Configurator::Instance().initializeSerial();
    Configurator::Instance().initializeSpiff();
    //pSpiffs_storage = new Spiffs_storage();
    //pSpiffs_storage -> remove(SPIFF_DRIVELOG_FILENAME);
//    pSpiffs_config = new Spiffs_config();

    //Delete the SPIFFS file configs if there are problems with the old configuration
    Logger::Instance().write(LogLevel::INFO, FPSTR("************* Configuring Modules"));
    Configurator::Instance().configs().load(new Custom_config(), true);
    //Normal loading of saved configurations from spiffs
    //Configurator::Instance().configs().load(new Custom_config());
    Logger::Instance().write(LogLevel::INFO, FPSTR("************* Creating Module Instances"));
    getFactoryInstances(Configurator::Instance().configs().all());
    Logger::Instance().write(LogLevel::INFO, FPSTR("************* Clearing Configurations"));
    Configurator::Instance().configs().clear();
    Logger::Instance().write(LogLevel::INFO, FPSTR("************* Configuring EventRules"));
    mEventRules = new Custom_eventrules(mModules);
    Logger::Instance().write(LogLevel::INFO, FPSTR("************* Setting up Modules"));
    mModules -> setup();
    mModules -> listEnabled();
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    Logger::Instance().write(LogLevel::INFO, FPSTR("************* Finished setting up Faraday Motion Pacer v"), String(mVersion));
    mIsSetup = true;
  }
}

void FMV::loop()
{
  mModules -> loop();
  if (pSimpleTimerPingPong -> check())
  {
    Logger::Instance().write(LogLevel::INFO, FPSTR("PINGPONG"));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
  }
}

void FMV::moduleEvent(Modulebase* sender, byte eventId)
{
  //DISABLE FOR NOW
  //return;
  mEventRules -> moduleEvent(sender, eventId);
}

void FMV::getFactoryInstances(std::vector<Configbase*> mConfigArray)
{
  for (int i=0; i<mConfigArray.size(); i++)
  {
    int configuration = mConfigArray[i] -> configuration;
    byte id = mConfigArray[i] -> id;

    Modulebase* fb = Modulefactory::getModuleInstance(id, configuration, this);
    mModules -> add(fb);

    switch(configuration) {
      case Configurations::WHEEL_CONFIG :
        Logger::Instance().write(LogLevel::DEBUG, FPSTR("getWheelInstance WHEEL_CONFIG"));
        wheelArray.push_back(new Wheel(id));
        break;
    }
  }
}
