#ifndef MotorController_h
#define MotorController_h

#include "dep/bldc_uart_comm_stm32f4_discovery/packet.h"
#include "dep/bldc_uart_comm_stm32f4_discovery/buffer.h"
#include "dep/bldc_uart_comm_stm32f4_discovery/crc.h"
#include "dep/bldc_uart_comm_stm32f4_discovery/datatypes.h"



class MotorController
{
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
  void get_values();

  void exists();
};

#endif
