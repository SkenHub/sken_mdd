/*
 * mdd.cpp
 *
 *  Created on: 2021/01/25
 *      Author: TakumaNakao
 */

#include "mdd.h"

Mdd::Mdd(void) :
	wheel_radius(40),
	turning_radius(300)
{
}

void Mdd::init(CommunicationMode mode,int period,uint8_t id)
{
	communication_mode = mode;
	if(mode == UART_MODE){
		communication_module.init(mode);
	}else if(mode == CAN_MODE){
		communication_module.init(mode,id);
	}
	motor_control_module.init(period);
}

void Mdd::update(void)
{
	if (communication_mode == UART_MODE) {
		if (communication_module.uartDataRead()) {
			ModeProcess(communication_module.getData());
		}
	}
	else if (communication_mode == CAN_MODE) {
		if(communication_module.canDataRead()){
			ModeProcess(communication_module.getData());
		}
	}
	motor_control_module.control();
}

void Mdd::ModeProcess(const receive_data_t& receive_data)
{
	//モータ制御データを取得し，必要箇所のみ書き換えて再登録する
	motor_control_data_t motor_control_data = motor_control_module.getMotorControlData();
	switch (receive_data.id) {
	case MOTOR_RPS_COMMAND_MODE:
		for (int i = 0; i < 4; i++) {
			motor_control_data.motor_data[i].target_rps = receive_data.data[i];
		}
		motor_control_module.setConfig(motor_control_data);
		break;
	case MOTOR_PWM_COMMAND_MODE:
		for (int i = 0; i < 4; i++) {
			motor_control_data.motor_data[i].pwm_command = receive_data.data[i];
		}
		motor_control_module.setConfig(motor_control_data);
		break;
	case MECANUM_MODE:
		mecanumCalc(receive_data.data[0],receive_data.data[1],receive_data.data[2],motor_control_data);
		break;
	case OMNI3_MODE:
		omni3Calc(receive_data.data[0],receive_data.data[1],receive_data.data[2],motor_control_data);
		break;
	case OMNI4_MODE:
		omni4Calc(receive_data.data[0],receive_data.data[1],receive_data.data[2],motor_control_data);
		break;
	case M1_PID_GAIN_CONFIG:
		motor_control_data.pid_gain[0].kp = receive_data.data[0];
		motor_control_data.pid_gain[0].ki = receive_data.data[1];
		motor_control_data.pid_gain[0].kd = receive_data.data[2];
		motor_control_data.pid_gain[0].time_constant = receive_data.data[3];
		motor_control_module.setConfig(motor_control_data);
		break;
	case M2_PID_GAIN_CONFIG:
		motor_control_data.pid_gain[1].kp = receive_data.data[0];
		motor_control_data.pid_gain[1].ki = receive_data.data[1];
		motor_control_data.pid_gain[1].kd = receive_data.data[2];
		motor_control_data.pid_gain[1].time_constant = receive_data.data[3];
		motor_control_module.setConfig(motor_control_data);
		break;
	case M3_PID_GAIN_CONFIG:
		motor_control_data.pid_gain[2].kp = receive_data.data[0];
		motor_control_data.pid_gain[2].ki = receive_data.data[1];
		motor_control_data.pid_gain[2].kd = receive_data.data[2];
		motor_control_data.pid_gain[2].time_constant = receive_data.data[3];
		motor_control_module.setConfig(motor_control_data);
		break;
	case M4_PID_GAIN_CONFIG:
		motor_control_data.pid_gain[3].kp = receive_data.data[0];
		motor_control_data.pid_gain[3].ki = receive_data.data[1];
		motor_control_data.pid_gain[3].kd = receive_data.data[2];
		motor_control_data.pid_gain[3].time_constant = receive_data.data[3];
		motor_control_module.setConfig(motor_control_data);
		break;
	case ROBOT_DIAMETER_CONFIG: //直径がおくられてくるので2分の1する
		wheel_radius = receive_data.data[0] / 2;
		turning_radius = receive_data.data[1] / 2;
		break;
	case PID_RESET_COMMAND:
		motor_control_module.resetPid();
		break;
	case MOTOR_COMMAND_MODE_SELECT:
		for (int i = 0; i < 4; i++) {
			motor_control_data.motor_data[i].pwm_mode = receive_data.data[i];
		}
		motor_control_module.setConfig(motor_control_data);
		break;
	case ENCODER_RESOLUTION_CONFIG:
		for (int i = 0; i < 4; i++) {
			motor_control_data.motor_data[i].encoder_resolution = receive_data.data[i];
		}
		motor_control_module.setConfig(motor_control_data);
		break;
	}
}

motor_control_data_t Mdd::getMotorControlData(void)
{
	return motor_control_module.getMotorControlData();
}

receive_data_t Mdd::getReceiveData(void)
{
	return communication_module.getData();
}

Can_data Mdd::getReceiveCanData(void){
	return communication_module.getCanData();
}

void Mdd::sendData(void)
{
	if (communication_mode == UART_MODE) {
		communication_module.uartDataSend();
	}
	else if (communication_mode == CAN_MODE) { //未実装
		communication_module.canDataSend();
	}
}

void Mdd::mecanumCalc(double vx,double vy,double omega,motor_control_data_t& motor_control_data)
{
	omega = omega * M_PI / 180;
	motor_control_data.motor_data[0].target_rps = ((-vx + vy) / sqrt(2) + turning_radius * omega) / (2 * M_PI * wheel_radius);
	motor_control_data.motor_data[1].target_rps = ((-vx - vy) / sqrt(2) + turning_radius * omega) / (2 * M_PI * wheel_radius);
	motor_control_data.motor_data[2].target_rps = ((vx - vy) / sqrt(2) + turning_radius * omega) / (2 * M_PI * wheel_radius);
	motor_control_data.motor_data[3].target_rps = ((vx + vy) / sqrt(2) + turning_radius * omega) / (2 * M_PI * wheel_radius);
	motor_control_module.setConfig(motor_control_data);
}

void Mdd::omni3Calc(double vx,double vy,double omega,motor_control_data_t& motor_control_data)
{
	omega = omega * M_PI / 180;
	motor_control_data.motor_data[0].target_rps = (-vx + turning_radius * omega) / (2 * M_PI * wheel_radius);
	motor_control_data.motor_data[1].target_rps = (vx / 2 - vy * sqrt(3) / 2 + turning_radius * omega) / (2 * M_PI * wheel_radius);
	motor_control_data.motor_data[2].target_rps = (vx / 2 + vy * sqrt(3) / 2 + turning_radius * omega) / (2 * M_PI * wheel_radius);
	motor_control_module.setConfig(motor_control_data);
}

void Mdd::omni4Calc(double vx,double vy,double omega,motor_control_data_t& motor_control_data)
{
	omega = omega * M_PI / 180;
	motor_control_data.motor_data[0].target_rps = (-vx + turning_radius * omega) / (2 * M_PI * wheel_radius);
	motor_control_data.motor_data[1].target_rps = (-vy + turning_radius * omega) / (2 * M_PI * wheel_radius);
	motor_control_data.motor_data[2].target_rps = (vx + turning_radius * omega) / (2 * M_PI * wheel_radius);
	motor_control_data.motor_data[3].target_rps = (vy + turning_radius * omega) / (2 * M_PI * wheel_radius);
	motor_control_module.setConfig(motor_control_data);
}
