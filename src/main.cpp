#include <Arduino.h>
#include "FMV.h"
// #include "components/Communication/Console.h"
// #include "components/ConfigController/ConfigController.h"
// #include "components/Controller/ControllerManager.h"
// #include "components/Connection/ConnectionManager.h"


#include "components/Sensors/IMU10DOF/IMU10DOF.h"

#include "components/MotorController/MotorController.h"
#include "components/Communication/WebSocketCommunicator.h"
#include "components/Utility/Log.h"


/******************************************************/
/** TODO:: Have a class for pin configuration (.ini) **/
//#define PINEXTERNALRESET 16 // used in the brain only.
#define PINDEADSWITCH 12
#define I2CDEV_SERIAL_DEBUG
// #define PINSERVOESC 0     //not used
// #define PINSERVOBRAKE1 2  //not used
// #define PINSERVOBRAKE2 14 //not used
// #define PINPIXEL 3        //not use
/******************************************************/
/**********************************************************/

FMV fmv;

WebSocketCommunicator wsCommunicator(81);


/***********************************************/

void setup() {
  Serial.begin(115200);
  delay(600);
  // TODO: Wire is I2C connection. We should have it started somewhere else than here.
  Wire.begin();
  fmv.setup();
  // pinMode(13, INPUT_PULLUP);
  wsCommunicator.wss->begin();
  Log::Instance()->enableWebsocket(&wsCommunicator);
}


void loop() {
  fmv.loop();
  yield();
  wsCommunicator.wss->loop();
  yield();

  // Check if clients want to connect to Wifi AP Server.
  //connectionManager.handleClientConnections(); // TODO:: Abstract this in the connectionManager.h
  yield();
  //controllerManager.handleController();
  yield();
  // TODO:: Find a place for calling this method in a loop.
  //while (Serial.available() > 0) motorController.processUartByte(Serial.read());
  //while(logMetro.check() == 1) Log::Instance()->write();

}
