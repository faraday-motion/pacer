#include <Arduino.h> // for the min // TODO:: See if we need this here.
#include "NunchuckController.h"
#include "components/Utility/Log.h"
#include "components/Connection/Radio/Commands.h"
#define min(a,b) ((a)<(b)?(a):(b)) // TODO:: Figure out why we don't get the min() function here.


// Construct the NunchuckController and the AbstractController
NunchuckController::NunchuckController(Radio* radio, AbstractDevice device)
 : AbstractController(device)
{
  Log::Logger()->write(Log::Level::DEBUG, "Started Construction of NunchuckController: ");

  this->radio = radio;
  this->nunchuck = device; // TODO:: Convert the abstract device pointer into a concered RadioDevice object

  /**
   * Setiing the Metro Timers
   */
  this->connectionLostTimer = new Metro(_LOST_CONNECTION);
  this->connectionLostTimer->reset(); // just to be safe

  // Init.
  responsePacket.Id      = 0;
  responsePacket.Command = 0;
  responsePacket.Value1  = 0;
  responsePacket.Value2  = 0;
  responsePacket.Value3  = 0;
  responsePacket.Value4  = 0;
  responsePacket.Value5  = 0;

  // Init.
  requestPacket.Id      = 0;
  requestPacket.Command = 0;
  requestPacket.Value1  = 0;
  requestPacket.Value2  = 0;
  requestPacket.Value3  = 0;
  requestPacket.Value4  = 0;
  requestPacket.Value5  = 0;
  Log::Logger()->write(Log::Level::DEBUG, "Finished Construction of NunchuckController: ");
}


bool NunchuckController::handleController()
{
  // Start listening to the physical nunchuck
  this->radio->setAddress(nunchuck.address);
  this->radio->setChannel(nunchuck.channel);

  if (this->radio->tryReadBytes(&responsePacket)) // populate the responsePacket
  {
    this->connectionLostTimer->reset();
    delay(5);
    this->processResponse(); // populate the requestPacket
  }
  delay(5);
  this->radio->tryWriteBytes(&requestPacket);

  // connectionLostTimer per each physical device
  if (this->connectionLostTimer->check() == 1)
  {
    Log::Logger()->write(Log::Level::WARNING, "NunchuckController has lost connection to physical device");
    this->isInMotion = false; // Flagging not in motion to be able to unset as active.
    return false;
  }

  return true;
}


void NunchuckController::processResponse()
{
  switch (responsePacket.Command)
  {
    // Making sure the controller is still in idle mode.
    case CONFIRM_SET_MODE_IDLE:
      Log::Logger()->write(Log::Level::DEBUG, "CONFIRM_SET_MODE_IDLE");
      Log::Logger()->write(Log::Level::DEBUG, "SRC: Nunchuck CTRL");
      requestPacket.Command = SET_MODE_ACTIVE;
      break;
    // Controller was set as active.
    case CONFIRM_SET_MODE_ACTIVE:
      Log::Logger()->write(Log::Level::DEBUG, "CONFIRM_SET_MODE_ACTIVE");
      requestPacket.Command = REQUEST_INPUT;
      break;
    // Controller is sending input signal.
    case CTRL_HAS_INPUT:
      Log::Logger()->write(Log::Level::DEBUG, "CTRL_HAS_INPUT");
      Log::Logger()->write(Log::Level::DEBUG, "NunchuckController Read Sample: " + (String)responsePacket.Value2);

      // TODO:: Set constraints from config.
      byte s = map(responsePacket.Value2, 1, 240, 0, 100);

      if (s >= 45 && s <= 55)
        s = 50;

      processInput(s);

      // Keep Asking for inputs
      requestPacket.Command = REQUEST_INPUT;
      break;
  }

  // This gets triggered only on active controller. So it is safe to take the physical controller out of idle mode;
  if(responsePacket.Command == 44)
  {
    Log::Logger()->write(Log::Level::DEBUG, "Nunchuck is IDLE. Requesting Inputs");
    requestPacket.Command = 50; // request input values.
  }
  else if (responsePacket.Command == 55)
  {
    Log::Logger()->write(Log::Level::DEBUG, "NunchuckController Read Sample: " + (String)responsePacket.Value2);
  }
}

bool NunchuckController::enable()
{
    Log::Logger()->write(Log::Level::DEBUG, "NunchuckController in being enabled.");
    this->radio->setAddress(nunchuck.address);
    this->radio->setChannel(nunchuck.channel);
    requestPacket.Command = SET_MODE_ACTIVE;
    return true;
}

bool NunchuckController::disable()
{
 // TODO:: Implement disable command. Maybe just go back to pending.
 return true;
}
