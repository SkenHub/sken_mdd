/*
 * mdd.h
 *
 *  Created on: 2021/01/24
 *      Author: TakumaNakao
 */

#ifndef MDD_H_
#define MDD_H_

#include "sken_library/include.h"
#include "math.h"
#include "define_data_struct.h"
#include "communication.h"
#include "motor_control.h"

class Mdd {
	Communication communication_module;
	MotorControl motor_control_module;
	CommunicationMode communication_mode;
	float wheel_radius;
	float turning_radius;
	void mecanumCalc(double vx,double vy,double omega,motor_control_data_t& motor_control_data);
	void omni3Calc(double vx,double vy,double omega,motor_control_data_t& motor_control_data);
	void omni4Calc(double vx,double vy,double omega,motor_control_data_t& motor_control_data);
public:
	Mdd(void);
	void init(CommunicationMode mode,int period);
	void update(void);
	void uartModeProcess(const receive_data_t& data);
	motor_control_data_t getMotorControlData(void);
	receive_data_t getReceiveData(void);
	void sendData(void);
};

#endif /* MDD_H_ */
