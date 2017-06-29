#ifndef RadioDevice_h
#define RadioDevice_h
#include <Arduino.h>


// TOOD: We need to have an abstract class for a ConnectedDevice similar to the AbstractController.
struct RadioDevice {
  byte id[5]; // We only have 5 bytes for the ID
  byte address[5]; // We only have 5 bytes for the Address. Last byte declares read/write
  byte channel = 100;
  bool pending = false; // flag the the device is pending.
  byte type;
};

#endif
