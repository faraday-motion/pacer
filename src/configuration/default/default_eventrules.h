#ifndef DEFAULT_EVENTRULES_H
#define DEFAULT_EVENTRULES_H

#include <Arduino.h>
#include "../base/default_eventrulesbase.h"
#include "./configuration.h"
#include "../../modules/modulelist.h"
#include "../../enums/modules.h"

class Default_eventrules : public Default_eventrulesbase{
protected:
  ModuleList* mModules = nullptr;
public:
  Default_eventrules(ModuleList* modules) : Default_eventrulesbase() {
    mModules = modules;
  }

  void moduleEvent(Modulebase* sender, byte eventId)
  {
    //Hook up module events with commands
    //TODO make this dynamic from a config
    if (sender -> module() == Modules::WIFI_SIMPLE_CONTROL)
    {
      //Enable wifi for wifi simple control
      if (eventId == Control_module::Events::CONFIGURE)
      {
        mModules -> commandByType(Modules::WIFI_CONNECTION, (byte)Wifi_connection::Commands::CONNECTION_WIFI_AP);
      }
    }

    /*
    if (sender -> module() == Modules::WIFI_CONNECTION)
    {
      //Configure ap on startup
      if (eventId == Wifi_connection::Events::CONFIGURE)
      {
        mModules -> command(Modules::WIFI_CONNECTION, (byte)Wifi_connection::Commands::CONNECTION_WIFI_AP);
      }
    }
    */

    //Handle getting the time when connected to the internet
    if (sender -> module() == Modules::WIFI_CONNECTION)
    {
      if (eventId == Wifi_connection::Events::WIFI_STA_STARTED)
      {
        mModules -> commandByType(Modules::NTP_TIMESERVICE, (byte)Ntp_timeservice::Commands::GET_TIME);
      }
    }

    if (sender -> module() == Modules::POWER_LIMIT)
    {
      //Handle dead man switch
      if (eventId == Power_limit::Events::LIMIT_DEAD)
      {
        //Handle vehicle dead
        //Handle neopixels
        mModules -> commandByType(Modules::NEOPIXELS, (byte)Neopixels::Commands::VEHICLE_DEAD);
        mModules -> commandByType(Modules::ESP32_DIGITAL_LED, (byte)Esp32_digital_led::Commands::VEHICLE_DEAD);
        //Handle sta connection
        //mModules -> commandByType(Modules::WIFI_CONNECTION, (byte)Wifi_connection::Commands::CONNECTION_WIFI_STA);
      }
      else if (eventId == Power_limit::Events::LIMIT_ALIVE)
      {
        //Handle neopixels
        mModules -> commandByType(Modules::NEOPIXELS, (byte)Neopixels::Commands::VEHICLE_ALIVE);
        mModules -> commandByType(Modules::ESP32_DIGITAL_LED, (byte)Esp32_digital_led::Commands::VEHICLE_ALIVE);
        //Handle sta connection
        //mModules -> commandByType(Modules::WIFI_CONNECTION, (byte)Wifi_connection::Commands::CONNECTION_WIFI_AP);
      }
    }

    //Handle neopixels
    if (sender -> role() == Roles::CONTROL_MODULE)
    {
      //Handle dead man switch
      if (eventId == Control_module::Events::CONTROL_NONE)
      {
        mModules -> commandByType(Modules::NEOPIXELS, (byte)Neopixels::Commands::CONTROL_NONE);
        mModules -> commandByType(Modules::ESP32_DIGITAL_LED, (byte)Esp32_digital_led::Commands::CONTROL_NONE);
      }
      else if (eventId == Control_module::Events::DRIVE_POWER)
      {
        mModules -> commandByType(Modules::NEOPIXELS, (byte)Neopixels::Commands::DRIVE_POWER);
        mModules -> commandByType(Modules::ESP32_DIGITAL_LED, (byte)Esp32_digital_led::Commands::DRIVE_POWER);
      }
      else if (eventId == Control_module::Events::DRIVE_BRAKE)
      {
        mModules -> commandByType(Modules::NEOPIXELS, (byte)Neopixels::Commands::DRIVE_BRAKE);
        mModules -> commandByType(Modules::ESP32_DIGITAL_LED, (byte)Esp32_digital_led::Commands::DRIVE_BRAKE);
      }
      else if (eventId == Control_module::Events::DRIVE_NEUTRAL)
      {
        mModules -> commandByType(Modules::NEOPIXELS, (byte)Neopixels::Commands::DRIVE_NEUTRAL);
        mModules -> commandByType(Modules::ESP32_DIGITAL_LED, (byte)Esp32_digital_led::Commands::DRIVE_NEUTRAL);
      }
      else if (eventId == Control_module::Events::TURN_LEFT)
      {
        mModules -> commandByType(Modules::NEOPIXELS, (byte)Neopixels::Commands::TURN_LEFT);
        mModules -> commandByType(Modules::ESP32_DIGITAL_LED, (byte)Esp32_digital_led::Commands::TURN_LEFT);
      }
      else if (eventId == Control_module::Events::TURN_NEUTRAL)
      {
        mModules -> commandByType(Modules::NEOPIXELS, (byte)Neopixels::Commands::TURN_NEUTRAL);
        mModules -> commandByType(Modules::ESP32_DIGITAL_LED, (byte)Esp32_digital_led::Commands::TURN_NEUTRAL);
      }
      else if (eventId == Control_module::Events::TURN_RIGHT)
      {
        mModules -> commandByType(Modules::NEOPIXELS, (byte)Neopixels::Commands::TURN_RIGHT);
        mModules -> commandByType(Modules::ESP32_DIGITAL_LED, (byte)Esp32_digital_led::Commands::TURN_RIGHT);
      }
      else if (eventId == Control_module::Events::DRIVE_MODE_0)
      {
        mModules -> commandByType(Modules::POWER_LIMIT, (byte)Power_limit::Commands::DRIVE_MODE_0);
      }
      else if (eventId == Control_module::Events::DRIVE_MODE_20)
      {
        mModules -> commandByType(Modules::POWER_LIMIT, (byte)Power_limit::Commands::DRIVE_MODE_20);
      }
      else if (eventId == Control_module::Events::DRIVE_MODE_40)
      {
        mModules -> commandByType(Modules::POWER_LIMIT, (byte)Power_limit::Commands::DRIVE_MODE_40);
      }
      else if (eventId == Control_module::Events::DRIVE_MODE_60)
      {
        mModules -> commandByType(Modules::POWER_LIMIT, (byte)Power_limit::Commands::DRIVE_MODE_60);
      }
      else if (eventId == Control_module::Events::DRIVE_MODE_80)
      {
        mModules -> commandByType(Modules::POWER_LIMIT, (byte)Power_limit::Commands::DRIVE_MODE_80);
      }
      else if (eventId == Control_module::Events::DRIVE_MODE_100)
      {
        mModules -> commandByType(Modules::POWER_LIMIT, (byte)Power_limit::Commands::DRIVE_MODE_100);
      }
    }
  }
};

#endif
