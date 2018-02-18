#ifndef DEFAULT_EXTERNALCOMMANDS_H
#define DEFAULT_EXTERNALCOMMANDS_H

#include <Arduino.h>
#include "../../configuration/default/configuration.h"

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
  static const byte DRIVE_MODE_20 = 8;
  static const byte DRIVE_MODE_40 = 9;
  static const byte DRIVE_MODE_60 = 10;
  static const byte DRIVE_MODE_80 = 11;
  static const byte DRIVE_MODE_100 = EXTERNALCOMMANDS_LIMIT;
};

#endif
