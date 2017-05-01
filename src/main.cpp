// Required includes
#include <Metro.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include "components/Connection/Wifi.h"
#include "components/Controller/Controller.h"
#include "components/Controller/PhoneController.h"
#include "components/MotorController/MotorController.h"


//temp
#include "FS.h"
#include "components/ConfigController/ConfigController.h"
//temp

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


/******************************************************/
/** TODO:: Metro Timers need to be ported to class  **/
// Metro Timers

Metro metro250ms                   = Metro(250);
Metro metroControllerRead          = Metro(50) ;
Metro metroControllerCommunication = Metro(500); // TODO:: implement timer to check if the connection is lost.
/***********************************************/


/***********************************************/
/** TODO:: Wrap up in a builder of some sort  **/

// Independent Objects
Config config;
ConfigController configController(&config);

Wifi            wifi;
WiFiServer      wifiServer(8899); // TODO::Get the port from a config file. Not sure how to do it as it need to be in this global scope where we can't execute ConfigController::getConfig(&Config);
MotorController motorController;
Controller      controller(&configController);
PhoneController phoneController;
/***********************************************/


void setup() {
  Serial.begin(115200);
  configController.loadConfig();
  wifi.setup(&wifiServer, &configController);
  motorController.setup();
  controller.setup(&motorController);
  phoneController.setup(&controller, &wifi);
}


void loop() {
  // Check if clients want to connect to Wifi AP Server.
  while (metro250ms.check() == 1)          wifi.registerClient();
  while (metroControllerRead.check() == 1) phoneController.read();
  yield();
  while (Serial.available() > 0) motorController.processUartByte(Serial.read());
}
