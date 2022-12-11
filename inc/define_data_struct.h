/*
 * define_data_struct.h
 *
 *  Created on: 2021/01/24
 *      Author: TakumaNakao
 */

#ifndef DEFINE_DATA_STRUCT_H_
#define DEFINE_DATA_STRUCT_H_

enum CommunicationMode {
	UART_MODE = 0,
	CAN_MODE
};

enum {
	MOTOR_RPS_COMMAND_MODE = 0,
	MOTOR_PWM_COMMAND_MODE,
	MECANUM_MODE,
	OMNI3_MODE,
	OMNI4_MODE,
	M1_PID_GAIN_CONFIG,
	M2_PID_GAIN_CONFIG,
	M3_PID_GAIN_CONFIG,
	M4_PID_GAIN_CONFIG,
	ROBOT_DIAMETER_CONFIG,
	PID_RESET_COMMAND,
	MOTOR_COMMAND_MODE_SELECT,
	ENCODER_RESOLUTION_CONFIG
};

struct receive_data_t {
	uint8_t seq;
	uint8_t id;
	float data[4];
};

struct motor_data_t {
	bool pwm_mode;
	double pwm_command;
	double target_rps;
	double now_rps;
	double encoder_resolution;
};

struct pid_gain_data_t {
	double kp;
	double ki;
	double kd;
	double time_constant;
};

struct motor_control_data_t {
	int control_period;
	motor_data_t motor_data[4];
	pid_gain_data_t pid_gain[4];
};



#endif /* DEFINE_DATA_STRUCT_H_ */
