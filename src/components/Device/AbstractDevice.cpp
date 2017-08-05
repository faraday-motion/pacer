#include "AbstractDevice.h"

AbstractDevice::AbstractDevice()
{

}

AbstractDevice::AbstractDevice(_wiredDevice device)
{
  // ID
  id[0] = device.id[0];
  id[1] = device.id[1];
  id[2] = device.id[2];
  id[3] = device.id[3];
  id[4] = device.id[4];

  // Type
  type = device.type;

  // Priority
  priority = device.priority;

  // enalbed
  enabled  = device.enabled;

  // constraints
  accelConstraint = device.accelConstraint;
  brakeConstraint = device.brakeConstraint;

  //pending
  pending  = 0;



}
