/*
 * communication.cpp
 *
 *  Created on: 2021/01/24
 *      Author: TakumaNakao
 */

#include "communication.h"

Communication::Communication(void)// :
	//mode(UART_MODE),
	//send_data_ring_number(0),
	//complete_send_data_ring_number(0)
{
}

void Communication::init(CommunicationMode communicate_mode,uint8_t id)
{
	//通信モードを選択
	mode = communicate_mode;
	if (communicate_mode == UART_MODE) {
		uart.init(C10,C11,SERIAL4,115200);
		uart.startDmaRead(uart_receive_data_dma,42);
	}else if(communicate_mode == CAN_MODE){
		selectStdid(id);
		sken_system.startCanCommunicate(B13,B12,CAN_2);
		sken_system.addCanRceiveInterruptFunc(CAN_2,&can_data_);
	}
}

bool Communication::uartDataRead(void)
{
	uint8_t uart_receive_data[2][21] = {};
	for (int i = 0; i < 21; i++) {
		//データのスタートを検索
		if (uart_receive_data_dma[i] == 0xA5 && uart_receive_data_dma[i + 1] == 0xA5) {
			for (int j = 0; j < 21; j++) {
				uart_receive_data[0][j] = uart_receive_data_dma[i + j];
				uart_receive_data[1][j] = uart_receive_data_dma[(i + j + 21) % 42];
			}
			break;
		}
		if (i == 20) {
			//見つからなかった場合失敗を返す
			return false;
		}
	}
	uint8_t check_sum[2] = {};
	for (int i = 2; i < 20; i++) {
		//チェックサムを計算
		check_sum[0] += uart_receive_data[0][i];
		check_sum[1] += uart_receive_data[1][i];
	}
	for (int i = 0; i < 2; i++) {
		if (uart_receive_data[i][20] == check_sum[i]) { //チェックサムが一致した場合
			if (uart_receive_data[i][2] > before_seq[0] || (uart_receive_data[i][2] - before_seq[0]) < -100) { //新しいデータか確認
				receive_data.seq = uart_receive_data[i][2];
				receive_data.id = uart_receive_data[i][3];
				//共用体を使用して送られてきたデータを浮動小数点型に変換
				ConvertIntFloat cif;
				for (int j = 0; j < 4; j++) {
					cif.int_val = 0;
					for (int k = 0; k < 4; k++) {
						cif.uint8_val[k] = uart_receive_data[i][j * 4 + k + 4];
					}
					receive_data.data[j] = cif.float_val;
				}
				before_seq[0] = uart_receive_data[i][2];
				//届いたデータのseqを返送するため送信リングバッファに格納
				send_data_ring_number[0] = (send_data_ring_number[0] + 1) % SEND_DATA_RING_SIZE;
				send_data_ring_buffer[0][send_data_ring_number[0]] = uart_receive_data[i][2];
				return true;
			}
		}
		if (i == 1) {
			//チェックサムが一致しなかった場合失敗を返す
			return false;
		}
	}
}

void Communication::uartDataSend(void)
{
	while (send_data_ring_number[0] != complete_send_data_ring_number[0]) {
		//送信リングバッファ内の送っていないデータがなくなるまで送信処理を続ける
		complete_send_data_ring_number[0] = (complete_send_data_ring_number[0] + 1) % SEND_DATA_RING_SIZE;
		uint8_t send_data = send_data_ring_buffer[0][complete_send_data_ring_number[0]];
		uart.write(&send_data,1,10);
	}
}

bool Communication::canDataRead(void)
{
	if(can_data_.rx_stdid == my_stdid){
		can_num = (can_data_.rx_data[0]>>6)-1;
		mdd_can_data[can_num] = can_data_;
		uint8_t seq = mdd_can_data[can_num].rx_data[0]&0x3F;

		//チェックサム確認
		uint8_t check_sum = 0;
		for(int i=0; i<7; i++){
			check_sum += mdd_can_data[can_num].rx_data[i];
		}
		if(mdd_can_data[can_num].rx_data[7] != check_sum) return false;
		//新しいデータかを確認
		if(seq < before_seq[can_num] && seq-before_seq[can_num] > -25) return false;
		before_seq[can_num] = seq;
		//データ格納
		receive_data.id = mdd_can_data[0].rx_data[1];
		receive_data.seq = seq;
		ConvertIntFloat cif;
		for(int i=0; i<4; i++){
			if(i != 3){
				for(int j=0; j<4; j++){
					cif.uint8_val[j] = mdd_can_data[i].rx_data[j+3];
				}
			}else{
				cif.uint8_val[0] = mdd_can_data[1].rx_data[1];
				cif.uint8_val[1] = mdd_can_data[1].rx_data[2];
				cif.uint8_val[2] = mdd_can_data[2].rx_data[1];
				cif.uint8_val[3] = mdd_can_data[2].rx_data[2];
			}
			receive_data.data[i] = cif.float_val;
		}
		//届いたデータのseqを返送するため送信リングバッファに格納
		send_data_ring_number[can_num] = (send_data_ring_number[can_num] + 1) % SEND_DATA_RING_SIZE;
		send_data_ring_buffer[can_num][send_data_ring_number[can_num]] = mdd_can_data[can_num].rx_data[0];
		return true;
	}else{
		return false;
	}
}

void Communication::canDataSend(void){
	while (send_data_ring_number[can_num] != complete_send_data_ring_number[can_num]) {
		//送信リングバッファ内の送っていないデータがなくなるまで送信処理を続ける
		complete_send_data_ring_number[can_num] = (complete_send_data_ring_number[can_num] + 1) % SEND_DATA_RING_SIZE;
		uint8_t send_data = send_data_ring_buffer[can_num][complete_send_data_ring_number[can_num]];
		sken_system.canTransmit(CAN_2,my_stdid,&send_data,1,20);
	}
}

receive_data_t Communication::getData(void)
{
	return receive_data;
}

Can_data Communication::getCanData(void){
	return mdd_can_data[1];
}

void Communication::selectStdid(uint8_t id){
	switch(id){
	case 0: my_stdid = MDD_0; break;
	case 1: my_stdid = MDD_1; break;
	case 2:	my_stdid = MDD_2; break;
	case 3:	my_stdid = MDD_3; break;
	case 4:	my_stdid = MDD_4; break;
	case 5:	my_stdid = MDD_5; break;
	case 6: my_stdid = MDD_6; break;
	case 7:	my_stdid = MDD_7; break;
	case 8: my_stdid = MDD_8; break;
	case 9: my_stdid = MDD_9; break;
	case 10:my_stdid = MDD_A; break;
	case 11:my_stdid = MDD_B; break;
	case 12:my_stdid = MDD_C; break;
	case 13:my_stdid = MDD_D; break;
	case 14:my_stdid = MDD_E; break;
	case 15:my_stdid = MDD_F; break;
	}
}
