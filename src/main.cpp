#include "stm32f4xx.h"
#include "sken_library/include.h"
#include "define_data_struct.h"
#include "mdd.h"

Mdd mdd;

const int CONTROL_PERIOD = 1;

motor_control_data_t motor_control_data;
receive_data_t receive_data;

void control(void);

int main(void)
{
	sken_system.init();
	mdd.init(UART_MODE,CONTROL_PERIOD);
	sken_system.addTimerInterruptFunc(control,CONTROL_PERIOD);
	while (1) {
		motor_control_data = mdd.getMotorControlData();
		receive_data = mdd.getReceiveData();
		mdd.sendData();
	}
}

void control(void)
{
	mdd.update();
}
