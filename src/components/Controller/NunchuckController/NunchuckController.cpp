#include <Arduino.h> // for the min // TODO:: See if we need this here.
#include "NunchuckController.h"
#include "components/Utility/Log.h"
#define min(a,b) ((a)<(b)?(a):(b)) // TODO:: Figure out why we don't get the min() function here.


// Construct the NunchuckController and the AbstractController
NunchuckController::NunchuckController(ConfigController* configController, Radio* radio, AbstractDevice device)
 : AbstractController(configController, device)
{

  this->radio = radio;
  this->nunchuck = device; // TODO:: Convert the abstract device pointer into a concered RadioDevice object

  /**
   * Setiing the Metro Timers
   */
  this->connectionLostTimer = new Metro(_LOST_CONNECTION);

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

}


bool NunchuckController::handleController()
{

  if (this->radio->tryReadBytes(&responsePacket)) // populate the responsePacket
  {
    this->connectionLostTimer->reset();
    delay(5);
    this->processResponse(); // populate the requestPacket
  }
  delay(5);

  //this->printRequestPacket();
  this->radio->tryWriteBytes(&requestPacket);

  // connectionLostTimer per each physical device
  if (this->connectionLostTimer->check() == 1)
  {
    Serial.println("NunchuckController has lost connection");
    return false;
  }

  return true;
}


void NunchuckController::processResponse()
{
  // This gets triggered only on active controller. So it is safe to take the physical controller out of idle mode;
  if(responsePacket.Command == 44)
  {
    Serial.println("Physical controller still in IDLE mode. Requesting Inputs");
    requestPacket.Command = 50; // request input values.
  }
  else if (responsePacket.Command == 55)
  {
    Serial.print(":::::::::::::: Controller inputs = ");
    Serial.print(responsePacket.Value2);
    Serial.println();
    // This is hardcoded for
    byte s = map(responsePacket.Value2, 48, 200, 0, 100);
    if (s >= 45 && s <= 55)
    {
      s = 50;
    }
    Serial.println(s);
    processInput(s);
    // processInput(responsePacket.Value2);
  }
}

bool NunchuckController::enable()
{
    this->radio->changeDevice(nunchuck);
    requestPacket.Command = 50;
    return true;
}

bool NunchuckController::disable()
{
  // TODO:: Implement disable command. Maybe just go back to pending.
 this->radio->changeDevice(nunchuck);
 requestPacket.Command = 40;
 return true;
}



// Debug

void NunchuckController::printRequestPacket()
{
  Serial.println("Nunchuck Packet sent:: ");
  Serial.print(requestPacket.Id);
  Serial.print(" ");
  Serial.print(requestPacket.Command);
  Serial.print(" ");
  Serial.print(requestPacket.Value1);
  Serial.print(" ");
  Serial.print(requestPacket.Value2);
  Serial.print(" ");
  Serial.print(requestPacket.Value3);
  Serial.print(" ");
  Serial.print(requestPacket.Value4);
  Serial.print(" ");
  Serial.print(requestPacket.Value5);
  Serial.print(" ");

  Serial.print("WriteBytes: ");
  Serial.println(packetSize);
  Serial.println();
}

void NunchuckController::printResponsePacket()
{
  Serial.println("Nunchuck Packet received:: ");
  Serial.print(responsePacket.Id);
  Serial.print(" ");
  Serial.print(responsePacket.Command);
  Serial.print(" ");
  Serial.print(responsePacket.Value1);
  Serial.print(" ");
  Serial.print(responsePacket.Value2);
  Serial.print(" ");
  Serial.print(responsePacket.Value3);
  Serial.print(" ");
  Serial.print(responsePacket.Value4);
  Serial.print(" ");
  Serial.print(responsePacket.Value5);
  Serial.print(" ");
  Serial.println();

  Serial.print("WriteBytes: ");
  Serial.println(packetSize);
}

void NunchuckController::printAddresses()
{

}
