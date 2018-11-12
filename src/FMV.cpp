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
#include "./configuration/custom/custom_safe_config.h"
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

    incrementResetStats();
    if (mSafeMode)
      Logger::Instance().write(LogLevel::INFO, FPSTR("************* Running in SAFE MODE"));

    Configurator::Instance().initializeAnalog();
    Configurator::Instance().initializeSerial();
    Configurator::Instance().initializeSpiff();

    //Delete the SPIFFS file configs if there are problems with the old configuration
    Configurator::Instance().configs().clear(true);

    Logger::Instance().write(LogLevel::INFO, FPSTR("************* Configuring Modules"));
    if (mSafeMode)
      Configurator::Instance().configs().load(new Custom_safe_config(), true);
    else
      Configurator::Instance().configs().load(new Custom_config(), false);

    Logger::Instance().write(LogLevel::INFO, FPSTR("************* Enabling Commands"));
    addEnabledCommands();

    Logger::Instance().write(LogLevel::INFO, FPSTR("************* Creating Module Instances"));
    getFactoryInstances(Configurator::Instance().configs().all());

    if (!mSafeMode)
    {
      Logger::Instance().write(LogLevel::INFO, FPSTR("************* Configuring EventRules"));
      mEventRules = new Custom_eventrules(this);
    }

    Logger::Instance().write(LogLevel::INFO, FPSTR("************* Setting up Modules"));
    modules().setup();

    //No need for buffering anymore
    Logger::Instance().setBufferSize(0);
    modules().listEnabled();
    Logger::Instance().write(LogLevel::INFO, FPSTR("Free Heap: "), String(ESP.getFreeHeap()));
    //Should probably be done after a time delay of 30, once all has been looping a few times...
    successfullStart();

    Logger::Instance().write(LogLevel::INFO, FPSTR("************* Finished setting up Faraday Motion Pacer v"), String(mVersion));
    mIsSetup = true;
  }
}

void FMV::incrementResetStats()
{
  Logger::Instance().write(LogLevel::INFO, FPSTR("************* Setting up Preferences"));
  preferences.begin("pacer", false);
  //preferences.clear();
  //preferences.remove("restarts");
  unsigned int restartCounter = preferences.getUInt("restarts", 0);
  restartCounter++;
  preferences.putUInt("restarts", restartCounter);

  unsigned int failedstartCounter = preferences.getUInt("failedstart", 0);
  if (failedstartCounter > 3)
    mSafeMode = true;
  failedstartCounter++;
  preferences.putUInt("failedstart", failedstartCounter);

  preferences.end();
  sensors().setIntSensor("rst", restartCounter);
  Logger::Instance().write(LogLevel::INFO, FPSTR("Restarts: "), String(restartCounter));
  Logger::Instance().write(LogLevel::INFO, FPSTR("************* Finished setting up Preferences"));
}

void FMV::successfullStart()
{
  preferences.begin("pacer", false);
  unsigned int failedstartCounter = preferences.getUInt("failedstart", 0);
  failedstartCounter = 0;
  preferences.putUInt("failedstart", failedstartCounter);
  preferences.end();
  sensors().setIntSensor("fst", failedstartCounter);
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
  Configurator::Instance().addCommand(ExternalCommands::GET_CONFIG);
}

void FMV::loop()
{
  mModules -> loop();
  if (pSimpleTimerPingPong -> check())
  {
    Logger::Instance().write(LogLevel::INFO, FPSTR("PINGPONG"));
    Logger::Instance().write(LogLevel::INFO, FPSTR("************* Free Heap: "), String(ESP.getFreeHeap()));
    Logger::Instance().write(LogLevel::INFO, FPSTR("************* Reset Count: "), String(sensors().getIntSensor("rst") -> getValue()));
  }
}

void FMV::moduleEvent(IModule * sender, byte eventId)
{
  Logger::Instance().write(LogLevel::DEBUG, FPSTR("FMV::moduleEvent Module: "), String(sender -> module()) + "eventId: " + String(eventId));
  if (mEventRules != nullptr)
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
