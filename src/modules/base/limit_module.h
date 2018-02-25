#ifndef LIMIT_MODULE_H
#define LIMIT_MODULE_H
#include <Arduino.h>
#include "./base.hpp"
#include "../../vehiclecontrol.h"

class Limit_module : public Modulebase  {
private:
protected:
  Vehiclecontrol mInputControl;
  Vehiclecontrol mOutputControl;
  Limit_module(byte id, int module) : Modulebase(id, module, Roles::LIMIT_MODULE) {
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
