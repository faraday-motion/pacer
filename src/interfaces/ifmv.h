#ifndef IFMV_H
#define IFMV_H

#include <vector>
#include <Arduino.h>
#include "./interfaces.hpp"

class IFMV : public virtual ILoopable {
   public:
     virtual IModuleList &modules() = 0;
     virtual ISensorList &sensors() = 0;
     virtual void moduleEvent(IModule * sender, byte eventId) = 0;
     virtual std::vector<IWheel*> getWheelValues() const = 0;
};
#endif
