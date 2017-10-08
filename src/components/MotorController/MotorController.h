#ifndef MotorController_h
#define MotorController_h

#include "packet.h"
#include "buffer.h"
#include "crc.h"
#include "datatypes.h"
#include "components/Config/Config.h"


class MotorController
{

private:
  byte motorCount;
public:

  MotorController();

  void setup();
  // Communication
  //void uartInit(void(*s_func)(unsigned char *data, unsigned int len), void(*p_func)(unsigned char b), void(*v_func)(mc_values *val));
  void uartInit();
  void processUartByte(unsigned char b);
  void update();

  // Motor Control
  void set_current(float current);
  void set_current_brake(float current);

  // Communication Commands
  void write(unsigned char *data, unsigned int len);
  void read();
  void get_values();
};

#endif
