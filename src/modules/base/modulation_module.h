#ifndef MODULATION_MODULE_H
#define MODULATION_MODULE_H
#include <Arduino.h>
#include "./base.hpp"
#include "../../enums/enums.hpp"
#include "../../vehiclecontrol.h"

class Modulation_module : public Modulebase  {
private:
protected:
  Vehiclecontrol mInputControl;
  Vehiclecontrol mOutputControl;
  Modulation_module(byte id, int module) : Modulebase(id, module, Roles::MODULATION_MODULE) {
    mInputControl.setName("Modulation_module-INPUT");
    mOutputControl.setName("Modulation_module-OUTPUT");
  }
public:

  const Vehiclecontrol getOutputControl() const
  {
    return mOutputControl;
  }
};

#endif
