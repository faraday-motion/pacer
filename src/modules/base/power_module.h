#ifndef POWER_MODULE_H
#define POWER_MODULE_H
#include <Arduino.h>
#include "./base.hpp"
#include "../../vehiclecontrol.h"

class Power_module : public Modulebase  {
private:
protected:
  Vehiclecontrol mInputControl;
  Power_module(byte id, int module) : Modulebase(id, module, Roles::POWER_MODULE) {
    mInputControl.setName("Power_module");
  }
public:

};

#endif
