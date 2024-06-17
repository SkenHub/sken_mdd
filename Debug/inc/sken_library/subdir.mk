################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../inc/sken_library/I2C.cpp \
../inc/sken_library/PS3.cpp \
../inc/sken_library/adc.cpp \
../inc/sken_library/can.cpp \
../inc/sken_library/debugger.cpp \
../inc/sken_library/encoder.cpp \
../inc/sken_library/gpio.cpp \
../inc/sken_library/gyro.cpp \
../inc/sken_library/io_name.cpp \
../inc/sken_library/pid.cpp \
../inc/sken_library/rc_pwm.cpp \
../inc/sken_library/system.cpp \
../inc/sken_library/uart.cpp 

OBJS += \
./inc/sken_library/I2C.o \
./inc/sken_library/PS3.o \
./inc/sken_library/adc.o \
./inc/sken_library/can.o \
./inc/sken_library/debugger.o \
./inc/sken_library/encoder.o \
./inc/sken_library/gpio.o \
./inc/sken_library/gyro.o \
./inc/sken_library/io_name.o \
./inc/sken_library/pid.o \
./inc/sken_library/rc_pwm.o \
./inc/sken_library/system.o \
./inc/sken_library/uart.o 

CPP_DEPS += \
./inc/sken_library/I2C.d \
./inc/sken_library/PS3.d \
./inc/sken_library/adc.d \
./inc/sken_library/can.d \
./inc/sken_library/debugger.d \
./inc/sken_library/encoder.d \
./inc/sken_library/gpio.d \
./inc/sken_library/gyro.d \
./inc/sken_library/io_name.d \
./inc/sken_library/pid.d \
./inc/sken_library/rc_pwm.d \
./inc/sken_library/system.d \
./inc/sken_library/uart.d 


# Each subdirectory must supply rules for building sources it contributes
inc/sken_library/%.o: ../inc/sken_library/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU G++ Compiler'
	@echo $(PWD)
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 -DSTM32F4 -DSTM32F446RETx -DNUCLEO_F446RE -DDEBUG -DSTM32F446xx -DUSE_HAL_DRIVER -I"C:/Users/hhhot/workspace/stm_project/sken_mdd/HAL_Driver/Inc/Legacy" -I"C:/Users/hhhot/workspace/stm_project/sken_mdd/Utilities/STM32F4xx-Nucleo" -I"C:/Users/hhhot/workspace/stm_project/sken_mdd/inc" -I"C:/Users/hhhot/workspace/stm_project/sken_mdd/CMSIS/device" -I"C:/Users/hhhot/workspace/stm_project/sken_mdd/CMSIS/core" -I"C:/Users/hhhot/workspace/stm_project/sken_mdd/HAL_Driver/Inc" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fno-exceptions -fno-rtti -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


