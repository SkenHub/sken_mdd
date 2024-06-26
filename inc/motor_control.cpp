/*
 * motor_control.cpp
 *
 *  Created on: 2021/01/24
 *      Author: TakumaNakao
 */

#include "motor_control.h"

MotorControl::MotorControl(void)
{
	for (int i = 0; i < 4; i++) {
		motor_control_data.control_period = 1;
		motor_control_data.motor_data[i].pwm_mode = 1;
		motor_control_data.motor_data[i].pwm_command = 0;
		motor_control_data.motor_data[i].target_rps = 0;
		motor_control_data.motor_data[i].now_rps = 0;
		motor_control_data.motor_data[i].encoder_resolution = 8192;
		motor_control_data.pid_gain[i].kp = 0;
		motor_control_data.pid_gain[i].ki = 0;
		motor_control_data.pid_gain[i].kd = 0;
		motor_control_data.pid_gain[i].time_constant = 0;
	}
}

void MotorControl::init(int period)
{
	motor_control_data.control_period = period;
	motor[0].init(Apin,B15,TIMER12 ,CH2);
	motor[0].init(Bpin,B14,TIMER12 ,CH1);
	motor[1].init(Apin,A11,TIMER1 ,CH4);
	motor[1].init(Bpin,A8 ,TIMER1 ,CH1);
	motor[2].init(Apin,A7 ,TIMER14,CH1);
	motor[2].init(Bpin,A6 ,TIMER13,CH1);
	motor[3].init(Apin,B9 ,TIMER11,CH1);
	motor[3].init(Bpin,B8 ,TIMER10,CH1);
	encoder[0].init(A0,A1,TIMER5);
	encoder[1].init(A5,B3,TIMER2);
	encoder[2].init(B6,B7,TIMER4);
	encoder[3].init(C6,C7,TIMER3);
}

void MotorControl::setConfig(const motor_control_data_t& data)
{
	for (int i = 0; i < 4; i++) {
		motor_control_data.motor_data[i].pwm_mode = data.motor_data[i].pwm_mode;
		motor_control_data.motor_data[i].pwm_command = data.motor_data[i].pwm_command;
		motor_control_data.motor_data[i].target_rps = data.motor_data[i].target_rps;
		motor_control_data.motor_data[i].encoder_resolution = data.motor_data[i].encoder_resolution;
		motor_control_data.pid_gain[i].kp = data.pid_gain[i].kp;
		motor_control_data.pid_gain[i].ki = data.pid_gain[i].ki;
		motor_control_data.pid_gain[i].kd = data.pid_gain[i].kd;
		motor_control_data.pid_gain[i].time_constant = data.pid_gain[i].time_constant;
		pid_controller[i].setGain(motor_control_data.pid_gain[i].kp,motor_control_data.pid_gain[i].ki,motor_control_data.pid_gain[i].kd,motor_control_data.pid_gain[i].time_constant);
	}
}

void MotorControl::control(void)
{
	//現在の回転数を取得
	for (int i = 0; i < 4; i++) {
		motor_control_data.motor_data[i].now_rps = -encoder[i].read() / motor_control_data.motor_data[i].encoder_resolution * 1000 / (double)motor_control_data.control_period;
		encoder[i].reset();
	}
	for (int i = 0; i < 4; i++) {
		if (motor_control_data.motor_data[i].pwm_mode) { //pwm_modeならそのまま出力
			motor[i].write(motor_control_data.motor_data[i].pwm_command);
		}
		else { //pid_modeでないならPID制御器を通して回転数制御を行う
			motor[i].write(pid_controller[i].control(motor_control_data.motor_data[i].target_rps,motor_control_data.motor_data[i].now_rps,motor_control_data.control_period));
		}
	}
}

void MotorControl::resetPid(void)
{
	for (int i = 0; i < 4; i++) {
		pid_controller[i].reset();
	}
}

motor_control_data_t MotorControl::getMotorControlData(void)
{
	return motor_control_data;
}
