#include <Arduino.h>
#include "FMV.h"


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

FMV fmv;
/***********************************************/

void setup() {
  Serial.begin(115200);
  fmv.setup();;

}
void loop() {
  fmv.loop();
  yield();
}
