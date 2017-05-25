#include "Console.h"


//IDEA:: Overload packet struct. e.g. ControlPacket vs DebugPacket etc.
Console::Console()
{

}

void Console::processCommand(ControllerPacket responsePacket, ControllerPacket* requestPacket)
{
  // TODO:: Remember by default we need to send command 1 - Request Name
  if (responsePacket.Command == 5)
  {
    Serial.println("Name Received");
    // TODO:: The device should also send it's type: NunchuckController = 2, PhoneController = 1
    // TODO:: Find a better way to do this..
    byte controllerId[5] = {responsePacket.Value1, responsePacket.Value2, responsePacket.Value3, responsePacket.Value4, responsePacket.Value5 };
    //bool isRegistered = controllerManager->registerController(2, controllerId);
    // if(isRegistered) {
    //   requestPacket->Command = 10;
    // }
  }
}
