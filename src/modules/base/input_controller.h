#ifndef INPUT_CONTROLLER_H
#define INPUT_CONTROLLER_H
#include <Arduino.h>
#include "./modulebase.h"
#include "../../enums/enums.hpp"
#include "../../vehiclecontrol.h"

class Input_controller : public Modulebase  {
private:
protected:
  Vehiclecontrol mInputControl;
  Input_controller(byte id, Modules module) : Modulebase(id, module, Roles::INPUT_CONTROLLER) {
    mInputControl.setName("Input_controller");
  }
public:

};

#endif
