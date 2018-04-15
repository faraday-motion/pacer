#ifndef IMODULELIST_H
#define IMODULELIST_H

#include <Arduino.h>
#include "./interfaces.hpp"
#include "../enums/roles.h"

class IModuleList : public virtual ILoopable
{
   public:
      virtual void commandByType(int module, byte command) = 0;
      virtual void commandById(byte id, byte command) = 0;
      virtual IModule * getByType(int module) = 0;
      virtual void add(IModule * fb) = 0;
      virtual IModule * get(byte id) = 0;
      virtual void listEnabled() = 0;
      virtual IControlModule * getActiveControl() = 0;
      virtual void setActiveController(byte id) = 0;
      virtual void deactivateControllers() = 0;
      virtual IModule * getEnabledByRole(Roles role) = 0;
};
#endif
