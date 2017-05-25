#ifndef ControllerPacket_h
#define ControllerPacket_h
#include <Arduino.h>

struct ControllerPacket {
 byte Id;
 byte Command;
 byte Value1 ;
 byte Value2 ;
 byte Value3 ;
 byte Value4 ;
 byte Value5 ;
};
#endif
