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
	Can_data can_data_,mdd_can_data[3];
	uint8_t can_num;
	MddStdid my_stdid;
	receive_data_t receive_data;
	CommunicationMode mode;
	int before_seq[3] = {-1,-1,-1};
	int send_data_ring_number[3];
	int complete_send_data_ring_number[3];
	uint8_t send_data_ring_buffer[3][SEND_DATA_RING_SIZE];
	void selectStdid(uint8_t id);
public:
	Communication(void);
	void init(CommunicationMode communicate_mode,uint8_t id = 0);
	bool uartDataRead(void);
	void uartDataSend(void);
	bool canDataRead(void);
	void canDataSend(void);
	receive_data_t getData(void);
	Can_data getCanData(void);
};


#endif /* COMMUNICATION_H_ */
