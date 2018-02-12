#ifndef INPUT_MODULATION_H
#define INPUT_MODULATION_H
#include <Arduino.h>
#include "./modulebase.h"
#include "../../enums/enums.hpp"
#include "../../vehiclecontrol.h"

class Input_modulation : public Modulebase  {
private:
protected:
  Vehiclecontrol mInputControl;
  Vehiclecontrol mOutputControl;
  Input_modulation(byte id, Modules module) : Modulebase(id, module, Roles::INPUT_MODULATION) {
    mInputControl.setName("Input_modulation-INPUT");
    mOutputControl.setName("Input_modulation-OUTPUT");
  }
public:

  const Vehiclecontrol getOutputControl() const
  {
    return mOutputControl;
  }
};

#endif
