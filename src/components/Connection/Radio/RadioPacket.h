#ifndef RadioPacket_h
#define RadioPacket_h
#include <Arduino.h>

struct RadioPacket {
 byte Id;
 byte Command;
 byte Value1 ;
 byte Value2 ;
 byte Value3 ;
 byte Value4 ;
 byte Value5 ;
};
#endif
