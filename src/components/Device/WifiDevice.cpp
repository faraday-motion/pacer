#include "WifiDevice.h"

void WifiDevice::reset()
{
  Serial.println("Clearing Wifi Pending Device");
  // Setting back to default
  for (byte i = 0; i < 5; i++) {
    id[i] = 0;
  }
  pending = false;
  priority = 0;
  type = 0;
}
