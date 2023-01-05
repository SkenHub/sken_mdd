#include "stm32f4xx.h"
#include "sken_library/include.h"
#include "define_data_struct.h"
#include "mdd.h"

Mdd mdd;

const int CONTROL_PERIOD = 1;

Gpio sw[4];
uint8_t mdd_id;

motor_control_data_t motor_control_data;
receive_data_t receive_data;
Can_data can_data;

bool flag;

void control(void);

int main(void)
{
	sken_system.init();
	sw[0].init(C3,INPUT_PULLUP);
	sw[1].init(C2,INPUT_PULLUP);
	sw[2].init(C1,INPUT_PULLUP);
	sw[3].init(C0,INPUT_PULLUP);

	if(!sw[0].read()){
		mdd.init(UART_MODE,CONTROL_PERIOD);
	}else{
		for(int i=0; i<3; i++){
			mdd_id |= ((!sw[i+1].read())<<(2-i));
		}
		mdd.init(CAN_MODE,CONTROL_PERIOD,mdd_id);
	}
	sken_system.addTimerInterruptFunc(control,CONTROL_PERIOD);
	while (1) {
		motor_control_data = mdd.getMotorControlData();
		receive_data = mdd.getReceiveData();
		can_data = mdd.getReceiveCanData();
		mdd.sendData();
	}
}

void control(void)
{
	mdd.update();
}
