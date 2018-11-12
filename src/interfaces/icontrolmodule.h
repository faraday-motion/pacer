#ifndef ICONTROLMODULE_H
#define ICONTROLMODULE_H

#include <Arduino.h>
#include "./interfaces.hpp"

class IControlModule : public virtual IActive, public virtual IClient, public virtual IRecieve {
   public:
};
#endif
