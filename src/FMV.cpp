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
#include "./sensors/sensors.hpp"

#include "./enums/modules.h"

void FMV::setup() {
  if (mIsSetup == false)
  {
    Logger::Instance().write(LogLevel::INFO, FPSTR("************* Setting up Faraday Motion Pacer v"),  String(mVersion));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Reset Reason Core 0: "), Tools::resetReason(rtc_get_reset_reason(0)));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Reset Reason Core 1: "), Tools::resetReason(rtc_get_reset_reason(1)));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Running Core: "), String(xPortGetCoreID()));
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));

    incrementRestarts();

    Configurator::Instance().initializeAnalog();
    Configurator::Instance().initializeSerial();
    Configurator::Instance().initializeSpiff();

    //Delete the SPIFFS file configs if there are problems with the old configuration
    Logger::Instance().write(LogLevel::INFO, FPSTR("************* Configuring Modules"));
    Configurator::Instance().configs().load(new Custom_config(), true);
    addEnabledCommands();
    //Normal loading of saved configurations from spiffs
    //Configurator::Instance().configs().load(new Custom_config());
    Logger::Instance().write(LogLevel::INFO, FPSTR("************* Creating Module Instances"));
    //Configurator::Instance().configs().swap(0, 1, true);
    getFactoryInstances(Configurator::Instance().configs().all());
    Logger::Instance().write(LogLevel::INFO, FPSTR("************* Configuring EventRules"));
    setEventRules(new Custom_eventrules());
    Logger::Instance().write(LogLevel::INFO, FPSTR("************* Setting up Modules"));
    modules().setup();
    //No need for buffering anymore
    Logger::Instance().setBufferSize(0);
    modules().listEnabled();
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    Logger::Instance().write(LogLevel::INFO, FPSTR("************* Finished setting up Faraday Motion Pacer v"), String(mVersion));
    mIsSetup = true;
  }
}

void FMV::incrementRestarts()
{
  Logger::Instance().write(LogLevel::INFO, FPSTR("************* Setting up Preferences"));
  preferences.begin("pacer", false);
  //preferences.clear();
  //preferences.remove("restarts");
  unsigned int counter = preferences.getUInt("restarts", 0);
  counter++;
  preferences.putUInt("restarts", counter);
  preferences.end();
  sensors().setIntSensor("RST", counter);
  Logger::Instance().write(LogLevel::INFO, FPSTR("Restarts: "), String(counter));
  Logger::Instance().write(LogLevel::INFO, FPSTR("************* Finished setting up Preferences"));
}

void FMV::addEnabledCommands()
{
  Configurator::Instance().addCommand(ExternalCommands::MODULE_ON);
  Configurator::Instance().addCommand(ExternalCommands::MODULE_OFF);
  Configurator::Instance().addCommand(ExternalCommands::DRIVE_POWER);
  Configurator::Instance().addCommand(ExternalCommands::DRIVE_BRAKE);
  Configurator::Instance().addCommand(ExternalCommands::TURN_LEFT);
  Configurator::Instance().addCommand(ExternalCommands::TURN_RIGHT);
  Configurator::Instance().addCommand(ExternalCommands::DRIVE_MODE_0);
  Configurator::Instance().addCommand(ExternalCommands::DRIVE_MODE_20);
  Configurator::Instance().addCommand(ExternalCommands::DRIVE_MODE_40);
  Configurator::Instance().addCommand(ExternalCommands::DRIVE_MODE_60);
  Configurator::Instance().addCommand(ExternalCommands::DRIVE_MODE_80);
  Configurator::Instance().addCommand(ExternalCommands::DRIVE_MODE_100);
  Configurator::Instance().addCommand(ExternalCommands::LED_ENABLED);
  Configurator::Instance().addCommand(ExternalCommands::LED_DISABLED);
}

void FMV::loop()
{
  mModules -> loop();
  if (pSimpleTimerPingPong -> check())
  {
    //Logger::Instance().write(LogLevel::INFO, FPSTR("PINGPONG"));
    Logger::Instance().write(LogLevel::INFO, FPSTR("************* Free Heap: "), String(ESP.getFreeHeap()));
  }
}

void FMV::moduleEvent(IModule * sender, byte eventId)
{
  Logger::Instance().write(LogLevel::DEBUG, FPSTR("FMV::moduleEvent Module: "), String(sender -> module()) + "eventId: " + String(eventId));
  mEventRules -> moduleEvent(sender, eventId);
}

void FMV::getFactoryInstances(std::vector<Configbase*> mConfigArray)
{
  for (int i=0; i<mConfigArray.size(); i++)
  {
    int configuration = mConfigArray[i] -> configuration;
    byte id = mConfigArray[i] -> getId();

    IModule * fb = Modulefactory::getModuleInstance(id, configuration, this);
    mModules -> add(fb);

    switch(configuration) {
      case Configurations::WHEEL_CONFIG :
        Logger::Instance().write(LogLevel::DEBUG, FPSTR("getWheelInstance WHEEL_CONFIG"));
        wheelArray.push_back(new Wheel(id));
        break;
    }
  }
}
