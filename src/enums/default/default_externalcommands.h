#ifndef DEFAULT_EXTERNALCOMMANDS_H
#define DEFAULT_EXTERNALCOMMANDS_H

#include <vector>
#include <Arduino.h>
#include "../../configuration/default/configuration.h"

#define EXTERNALCOMMAND_MODULE_ON "module_on"
#define EXTERNALCOMMAND_MODULE_OFF "module_off"
#define EXTERNALCOMMAND_DISABLE_CONTROLLERS "disable_controllers"
#define EXTERNALCOMMAND_ENABLE_CONTROLLER "enable_controller"
#define EXTERNALCOMMAND_DRIVE_POWER "drive_power"
#define EXTERNALCOMMAND_DRIVE_BRAKE "drive_brake"
#define EXTERNALCOMMAND_TURN_LEFT "turn_left"
#define EXTERNALCOMMAND_TURN_RIGHT "turn_right"
#define EXTERNALCOMMAND_DRIVE_MODE_0 "drive_mode_0"
#define EXTERNALCOMMAND_DRIVE_MODE_20 "drive_mode_20"
#define EXTERNALCOMMAND_DRIVE_MODE_40 "drive_mode_40"
#define EXTERNALCOMMAND_DRIVE_MODE_60 "drive_mode_60"
#define EXTERNALCOMMAND_DRIVE_MODE_80 "drive_mode_80"
#define EXTERNALCOMMAND_DRIVE_MODE_100 "drive_mode_100"
#define EXTERNALCOMMAND_LED_ENABLED "led_enabled"
#define EXTERNALCOMMAND_LED_DISABLED "led_disabled"

class Default_externalCommands {
public:
  static const byte MODULE_ON = 0;
  static const byte MODULE_OFF = 1;
  static const byte DISABLE_CONTROLLERS = 2;
  static const byte ENABLE_CONTROLLER = 3;
  static const byte DRIVE_POWER = 4;
  static const byte DRIVE_BRAKE = 5;
  static const byte TURN_LEFT = 6;
  static const byte TURN_RIGHT = 7;
  static const byte DRIVE_MODE_0 = 8;
  static const byte DRIVE_MODE_20 = 9;
  static const byte DRIVE_MODE_40 = 10;
  static const byte DRIVE_MODE_60 = 11;
  static const byte DRIVE_MODE_80 = 12;
  static const byte DRIVE_MODE_100 = 13;
  static const byte LED_ENABLED = 14;
  static const byte LED_DISABLED = 15;

  static byte get(String command)
  {
    if (command == EXTERNALCOMMAND_MODULE_ON)
      return MODULE_ON;
    else if (command == EXTERNALCOMMAND_MODULE_OFF)
      return MODULE_OFF;
    else if (command == EXTERNALCOMMAND_DISABLE_CONTROLLERS)
      return DISABLE_CONTROLLERS;
    else if (command == EXTERNALCOMMAND_ENABLE_CONTROLLER)
      return ENABLE_CONTROLLER;
    else if (command == EXTERNALCOMMAND_DRIVE_POWER)
      return DRIVE_POWER;
    else if (command == EXTERNALCOMMAND_DRIVE_BRAKE)
      return DRIVE_BRAKE;
    else if (command == EXTERNALCOMMAND_TURN_LEFT)
      return TURN_LEFT;
    else if (command == EXTERNALCOMMAND_TURN_RIGHT)
      return TURN_RIGHT;
    else if (command == EXTERNALCOMMAND_DRIVE_MODE_0)
      return DRIVE_MODE_0;
    else if (command == EXTERNALCOMMAND_DRIVE_MODE_20)
      return DRIVE_MODE_20;
    else if (command == EXTERNALCOMMAND_DRIVE_MODE_40)
      return DRIVE_MODE_40;
    else if (command == EXTERNALCOMMAND_DRIVE_MODE_60)
      return DRIVE_MODE_60;
    else if (command == EXTERNALCOMMAND_DRIVE_MODE_80)
      return DRIVE_MODE_80;
    else if (command == EXTERNALCOMMAND_DRIVE_MODE_100)
      return DRIVE_MODE_100;
    else if (command == EXTERNALCOMMAND_LED_ENABLED)
      return LED_ENABLED;
    else if (command == EXTERNALCOMMAND_LED_DISABLED)
      return LED_DISABLED;
  }

  static String get(byte command)
  {
    if (command == MODULE_ON)
      return EXTERNALCOMMAND_MODULE_ON;
    else if (command == MODULE_OFF)
      return EXTERNALCOMMAND_MODULE_OFF;
    else if (command == DISABLE_CONTROLLERS)
      return EXTERNALCOMMAND_DISABLE_CONTROLLERS;
    else if (command == ENABLE_CONTROLLER)
      return EXTERNALCOMMAND_ENABLE_CONTROLLER;
    else if (command == DRIVE_POWER)
      return EXTERNALCOMMAND_DRIVE_POWER;
    else if (command == DRIVE_BRAKE)
      return EXTERNALCOMMAND_DRIVE_BRAKE;
    else if (command == TURN_LEFT)
      return EXTERNALCOMMAND_TURN_LEFT;
    else if (command == TURN_RIGHT)
      return EXTERNALCOMMAND_TURN_RIGHT;
    else if (command == DRIVE_MODE_0)
      return EXTERNALCOMMAND_DRIVE_MODE_0;
    else if (command == DRIVE_MODE_20)
      return EXTERNALCOMMAND_DRIVE_MODE_20;
    else if (command == DRIVE_MODE_40)
      return EXTERNALCOMMAND_DRIVE_MODE_40;
    else if (command == DRIVE_MODE_60)
      return EXTERNALCOMMAND_DRIVE_MODE_60;
    else if (command == DRIVE_MODE_80)
      return EXTERNALCOMMAND_DRIVE_MODE_80;
    else if (command == DRIVE_MODE_100)
      return EXTERNALCOMMAND_DRIVE_MODE_100;
    else if (command == LED_ENABLED)
      return EXTERNALCOMMAND_LED_ENABLED;
    else if (command == LED_DISABLED)
      return EXTERNALCOMMAND_LED_DISABLED;
    else
      return "unknown";  
  }
private:
};

#endif
