/*
 * Motor.h
 *
 *  Created on: 2018/07/19
 *      Author: TakumaNakao
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include"sken_library/include.h"

class Motor {
	Gpio MOTOR_Pin_A;
	Gpio MOTOR_Pin_B;
	Gpio MOTOR_Pin_P;
public:
	void init(Pin pin_a,Pin pin_b,Pin pin_p,TimerNumber tim_num,TimerChannel tim_ch)
	{
		MOTOR_Pin_A.init(pin_a,OUTPUT);
		MOTOR_Pin_B.init(pin_b,OUTPUT);
		MOTOR_Pin_P.init(pin_p,PWM_OUTPUT,tim_num,tim_ch);
	}
	void turn(double p)
	{
		if (p > 0) {
			MOTOR_Pin_A.write(HIGH);
			MOTOR_Pin_B.write(LOW);
			MOTOR_Pin_P.write(p);
		}
		else if (p < 0) {
			MOTOR_Pin_A.write(LOW);
			MOTOR_Pin_B.write(HIGH);
			MOTOR_Pin_P.write(-p);
		}
		else {
			MOTOR_Pin_A.write(LOW);
			MOTOR_Pin_B.write(LOW);
			MOTOR_Pin_P.write(0);
		}
	}
	void free(void)
	{
		MOTOR_Pin_A.write(LOW);
		MOTOR_Pin_B.write(LOW);
		MOTOR_Pin_P.write(0);
	}
	void brake(void)
	{
		MOTOR_Pin_A.write(HIGH);
		MOTOR_Pin_B.write(HIGH);
		MOTOR_Pin_P.write(0);
	}
};

#endif /* MOTOR_H_ */
