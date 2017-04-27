#include <Arduino.h>
#include "MotorController.h"
#include "dep/bldc_uart_comm_stm32f4_discovery/packet.h"
#include "dep/bldc_uart_comm_stm32f4_discovery/packet.c"
#include "dep/bldc_uart_comm_stm32f4_discovery/buffer.h"
#include "dep/bldc_uart_comm_stm32f4_discovery/buffer.c"
#include "dep/bldc_uart_comm_stm32f4_discovery/crc.h"
#include "dep/bldc_uart_comm_stm32f4_discovery/crc.c"
#include "dep/bldc_uart_comm_stm32f4_discovery/bldc_interface.h"
#include "dep/bldc_uart_comm_stm32f4_discovery/bldc_interface.c"
#include "dep/bldc_uart_comm_stm32f4_discovery/bldc_interface_uart.h"
#include "dep/bldc_uart_comm_stm32f4_discovery/bldc_interface_uart.c"
#include "dep/bldc_uart_comm_stm32f4_discovery/datatypes.h"


// Settings NOT SURE IF NEEDED
//#define PACKET_HANDLER

MotorController::MotorController()
{

}


/**
 * Initialize the UART interface communication.
 *
 */
//void MotorController::uartInit(void(*s_func)(unsigned char *data, unsigned int len), void(*p_func)(unsigned char b), void(*v_func)(mc_values *val))
void MotorController::uartInit(void(*s_func)(unsigned char *data, unsigned int len))
{
	bldc_interface_uart_init(s_func);

  // TODO:: this function is setting the callback only for one command.
	//bldc_interface_set_rx_value_func(v_func);
}

/**
 * Processesing the bytes received from the VESC.
 * @param b incoming bytes
 */
void MotorController::uartProcess(unsigned char b)
{
	bldc_interface_uart_process_byte(b);
}

void MotorController::update()
{
	bldc_interface_uart_run_timer();
}

void MotorController::set_current(float current)
{
	bldc_interface_set_current(current);
}

void MotorController::set_current_brake(float current)
{
	bldc_interface_set_current_brake(current);
}

void MotorController::get_values()
{
	bldc_interface_get_values();
}

void MotorController::exists() {
	//Serial.println("MotorController Object Exists");
}
