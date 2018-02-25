#ifndef VESC_PACKET_H
#define VESC_PACKET_H
#include <functional>
#include <vector>
#include <Arduino.h>
#include "./ivesc_send_recieve.h"
#include "../../log/logger.h"

// Settings
#define PACKET_RX_TIMEOUT		2
#define PACKET_MAX_PL_LEN		512

class Vesc_packet
{
private:
	typedef struct {
		volatile unsigned char rx_state;
		volatile unsigned char rx_timeout;
		unsigned int payload_length;
		unsigned char rx_buffer[PACKET_MAX_PL_LEN];
		unsigned char tx_buffer[PACKET_MAX_PL_LEN + 6];
		unsigned int rx_data_ptr;
		unsigned char crc_low;
		unsigned char crc_high;
	} PACKET_STATE_t;
	PACKET_STATE_t handler_states;
	IVesc_send_recieve * pSendReciever = nullptr;
public:
	static std::vector<Vesc_packet*> vectorPackets;
	Vesc_packet(IVesc_send_recieve * sendReciever)
	{
		this -> pSendReciever = sendReciever;
		Vesc_packet::vectorPackets.push_back(this);
	}

	~Vesc_packet()
	{
		byte toDelete = -1;
		for (byte i=0; i<Vesc_packet::vectorPackets.size(); i++)
	  {
			if (Vesc_packet::vectorPackets[i] == this)
			{
				toDelete = i;
				break;
			}
		}
		if (toDelete > -1)
		{
			Vesc_packet::vectorPackets.erase(Vesc_packet::vectorPackets.begin() + toDelete);
		}
		this -> pSendReciever = nullptr;
	}

	void timerTimeout();
	void addTx(unsigned char *data, unsigned int len);
	void addRx(uint8_t rx_data);

	static void timerTimeouts()
	{
		for (byte i=0; i<Vesc_packet::vectorPackets.size(); i++)
		{
			 Vesc_packet::vectorPackets[i] -> timerTimeout();
		}
	}
};


#endif
