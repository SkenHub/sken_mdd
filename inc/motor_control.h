/*
 * motor_control.h
 *
 *  Created on: 2021/01/24
 *      Author: TakumaNakao
 */

#ifndef MOTOR_CONTROL_H_
#define MOTOR_CONTROL_H_

#include "sken_library/include.h"
#include "define_data_struct.h"
#include "motor.h"

class MotorControl {
	Motor motor[4];
	Encoder encoder[4];
	Pid pid_controller[4];
	motor_control_data_t motor_control_data;
public:
	MotorControl(void);
	void init(int period);
	void setConfig(const motor_control_data_t& data);
	void control(void);
	void resetPid(void);
	motor_control_data_t getMotorControlData(void);
};



#endif /* MOTOR_CONTROL_H_ */
