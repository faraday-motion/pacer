#ifndef COMMANDS_H
#define COMMANDS_H
#include "../configuration/default/configuration.h"

enum class ExternalCommands {
  MODULE_ON,
  MODULE_OFF,
  DISABLE_CONTROLLERS,
  ENABLE_CONTROLLER,
  DRIVE_POWER,
  DRIVE_BRAKE,
  TURN_LEFT,
  TURN_RIGHT,
  DRIVE_MODE_20,
  DRIVE_MODE_40,
  DRIVE_MODE_60,
  DRIVE_MODE_80,
  DRIVE_MODE_100 = EXTERNALCOMMANDS_LIMIT
};

#endif
