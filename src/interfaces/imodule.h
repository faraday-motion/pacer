#ifndef IMODULE_H
#define IMODULE_H

#include <Arduino.h>
#include "./interfaces.hpp"

//class IModule : public virtual ILoopable, public virtual IEnableable {
class IModule : public ILoopable, public IEnableable {
   public:
     virtual int module() = 0;
     virtual byte role() = 0;
     virtual byte id() = 0;
     virtual void command(byte command) = 0;
     virtual String getModuleName() = 0;
};
#endif
