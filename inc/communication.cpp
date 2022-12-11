/*
 * communication.cpp
 *
 *  Created on: 2021/01/24
 *      Author: TakumaNakao
 */

#include "communication.h"

Communication::Communication(void) :
	mode(UART_MODE),
	before_seq(-1),
	send_data_ring_number(0),
	complete_send_data_ring_number(0)
{
}

void Communication::init(CommunicationMode communicate_mode)
{
	mode = communicate_mode;
	if (communicate_mode == UART_MODE) {
		uart.init(B10,C5,SERIAL3,115200);
		uart.startDmaRead(uart_receive_data_dma,42);
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
			if (uart_receive_data[i][2] > before_seq || (uart_receive_data[i][2] - before_seq) < -100) { //新しいデータか確認
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
				before_seq = uart_receive_data[i][2];
				//届いたデータのseqを返送するため送信リングバッファに格納
				send_data_ring_number = (send_data_ring_number + 1) % SEND_DATA_RING_SIZE;
				send_data_ring_buffer[send_data_ring_number] = uart_receive_data[i][2];
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
	while (send_data_ring_number != complete_send_data_ring_number) {
		//送信リングバッファ内の送っていないデータがなくなるまで送信処理を続ける
		complete_send_data_ring_number = (complete_send_data_ring_number + 1) % SEND_DATA_RING_SIZE;
		uint8_t send_data = send_data_ring_buffer[complete_send_data_ring_number];
		uart.write(&send_data,1,10);
	}
}

receive_data_t Communication::getData(void)
{
	return receive_data;
}
