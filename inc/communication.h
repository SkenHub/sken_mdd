/*
 * communication.h
 *
 *  Created on: 2021/01/24
 *      Author: TakumaNakao
 */

#ifndef COMMUNICATION_H_
#define COMMUNICATION_H_

#include "sken_library/include.h"
#include "define_data_struct.h"

class Communication {
	static constexpr int SEND_DATA_RING_SIZE = 30;
	Uart uart;
	uint8_t uart_receive_data_dma[42];
	receive_data_t receive_data;
	CommunicationMode mode;
	int before_seq;
	int send_data_ring_number;
	int complete_send_data_ring_number;
	uint8_t send_data_ring_buffer[SEND_DATA_RING_SIZE];
public:
	Communication(void);
	void init(CommunicationMode communicate_mode);
	bool uartDataRead(void);
	void uartDataSend(void);
	receive_data_t getData(void);
};


#endif /* COMMUNICATION_H_ */
