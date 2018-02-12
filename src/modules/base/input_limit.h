#ifndef INPUT_LIMIT_H
#define INPUT_LIMIT_H
#include <Arduino.h>
#include "./modulebase.h"
#include "../../enums/enums.hpp"
#include "../../vehiclecontrol.h"

class Input_limit : public Modulebase  {
private:
protected:
  Vehiclecontrol mInputControl;
  Vehiclecontrol mOutputControl;
  Input_limit(byte id, Modules module) : Modulebase(id, module,Roles::INPUT_LIMIT) {
    mInputControl.setName("Input_limit-INPUT");
    mOutputControl.setName("Input_limit-OUTPUT");
  }
public:

  const Vehiclecontrol getOutputControl() const
  {
    return mOutputControl;
  }
};

#endif
