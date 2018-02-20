#include "../../macros.h"
#include "./vesc_packet.h"
#include "../../utility/tools.h"
#include <cstring>
#include <functional>

std::vector<Vesc_packet*> Vesc_packet::vectorPackets;

void Vesc_packet::addTx(unsigned char *data, unsigned int len) {
	if (len > PACKET_MAX_PL_LEN) {
		return;
	}
	int b_ind = 0;
	if (len <= 256) {
		handler_states.tx_buffer[b_ind++] = 2;
		handler_states.tx_buffer[b_ind++] = len;
	} else {
		handler_states.tx_buffer[b_ind++] = 3;
		handler_states.tx_buffer[b_ind++] = len >> 8;
		handler_states.tx_buffer[b_ind++] = len & 0xFF;
	}

	memcpy(handler_states.tx_buffer + b_ind, data, len);
	b_ind += len;

	unsigned short crc = Tools::crc16(data, len);
	handler_states.tx_buffer[b_ind++] = (uint8_t)(crc >> 8);
	handler_states.tx_buffer[b_ind++] = (uint8_t)(crc & 0xFF);
	handler_states.tx_buffer[b_ind++] = 3;

	if (pSendReciever != nullptr) {
		pSendReciever -> send(handler_states.tx_buffer, b_ind);
	}
}

void Vesc_packet::addRx(uint8_t rx_data) {
	switch (handler_states.rx_state) {
	case 0:
		if (rx_data == 2) {
			// 1 byte PL len
			handler_states.rx_state += 2;
			handler_states.rx_timeout = PACKET_RX_TIMEOUT;
			handler_states.rx_data_ptr = 0;
			handler_states.payload_length = 0;
		} else if (rx_data == 3) {
			// 2 byte PL len
			handler_states.rx_state++;
			handler_states.rx_timeout = PACKET_RX_TIMEOUT;
			handler_states.rx_data_ptr = 0;
			handler_states.payload_length = 0;
		} else {
			handler_states.rx_state = 0;
		}
		break;

	case 1:
		handler_states.payload_length = (unsigned int)rx_data << 8;
		handler_states.rx_state++;
		handler_states.rx_timeout = PACKET_RX_TIMEOUT;
		break;

	case 2:
		handler_states.payload_length |= (unsigned int)rx_data;
		if (handler_states.payload_length > 0 &&
				handler_states.payload_length <= PACKET_MAX_PL_LEN) {
			handler_states.rx_state++;
			handler_states.rx_timeout = PACKET_RX_TIMEOUT;
		} else {
			handler_states.rx_state = 0;
		}
		break;

	case 3:
		handler_states.rx_buffer[handler_states.rx_data_ptr++] = rx_data;
		if (handler_states.rx_data_ptr == handler_states.payload_length) {
			handler_states.rx_state++;
		}
		handler_states.rx_timeout = PACKET_RX_TIMEOUT;
		break;

	case 4:
		handler_states.crc_high = rx_data;
		handler_states.rx_state++;
		handler_states.rx_timeout = PACKET_RX_TIMEOUT;
		break;

	case 5:
		handler_states.crc_low = rx_data;
		handler_states.rx_state++;
		handler_states.rx_timeout = PACKET_RX_TIMEOUT;
		break;

	case 6:
		if (rx_data == 3) {
			if (Tools::crc16(handler_states.rx_buffer, handler_states.payload_length)
					== ((unsigned short)handler_states.crc_high << 8
							| (unsigned short)handler_states.crc_low)) {
				// Packet received!
				if (pSendReciever != nullptr) {
					pSendReciever -> recieve(handler_states.rx_buffer, handler_states.payload_length);
				}
			}
		}
		handler_states.rx_state = 0;
		break;

	default:
		handler_states.rx_state = 0;
		break;
	}
}

/**
 * Call this function every millisecond.
 */
void Vesc_packet::timerTimeout() {
	if (handler_states.rx_timeout) {
		handler_states.rx_timeout--;
	} else {
		handler_states.rx_state = 0;
	}
}
