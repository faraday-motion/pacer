#ifndef RadioPacket_h
#define RadioPacket_h
#include <Arduino.h>

struct RadioPacket {
 byte Id = 0;
 byte Command = 0;
 byte Value1  = 0;
 byte Value2  = 0;
 byte Value3  = 0;
 byte Value4  = 0;
 byte Value5  = 0;
};
#endif
