#include <Arduino.h>
#include "components/Connection/Wifi.h"
#include "components/Controller/ControllerManager.h"
#include "components/MotorController/MotorController.h"
#include "components/ConfigController/ConfigController.h"
#include "components/Communication/WebSocketCommunicator.h"
#include "components/Utility/Log.h"

/******************************************************/
/** TODO:: Have a class for pin configuration (.ini) **/
//#define PINEXTERNALRESET 16 // used in the brain only.
#define PINDEADSWITCH 12
// #define PINSERVOESC 0     //not used
// #define PINSERVOBRAKE1 2  //not used
// #define PINSERVOBRAKE2 14 //not used
// #define PINPIXEL 3        //not use
/******************************************************/
/**********************************************************/


/***********************************************/
/** TODO:: Wrap up in a builder of some sort  **/

// Independent Objects
Wifi              wifi;
ConfigController  configController;
ControllerManager controllerManager(&configController, &wifi);
WebSocketCommunicator wsCommunicator(81);

/***********************************************/

void setup() {
  Serial.begin(115200);
  configController.loadConfig();
  wifi.setup(&configController);

  // TODO:: This should be in a hanlder function.
  controllerManager.registerController(1, 3); // type = 1, id = 3
  controllerManager.setActiveController(3);
  wsCommunicator.wss->begin();
  Log::Instance()->enableWebsocket(&wsCommunicator);
}


void loop() {
  wsCommunicator.wss->loop();
  yield();
  // Check if clients want to connect to Wifi AP Server.
  wifi.handleClientConnections();
  yield();
  controllerManager.handleController();
  yield();
  // TODO:: Find a place for calling this method in a loop.
  //while (Serial.available() > 0) motorController.processUartByte(Serial.read());
  //while(logMetro.check() == 1) Log::Instance()->write();

}
