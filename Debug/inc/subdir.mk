################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../inc/communication.cpp \
../inc/mdd.cpp \
../inc/motor_control.cpp 

OBJS += \
./inc/communication.o \
./inc/mdd.o \
./inc/motor_control.o 

CPP_DEPS += \
./inc/communication.d \
./inc/mdd.d \
./inc/motor_control.d 


# Each subdirectory must supply rules for building sources it contributes
inc/%.o: ../inc/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU G++ Compiler'
	@echo $(PWD)
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 -DSTM32F4 -DSTM32F446RETx -DNUCLEO_F446RE -DDEBUG -DSTM32F446xx -DUSE_HAL_DRIVER -I"C:/Users/hotaka/workspace/sken_mdd/HAL_Driver/Inc/Legacy" -I"C:/Users/hotaka/workspace/sken_mdd/Utilities/STM32F4xx-Nucleo" -I"C:/Users/hotaka/workspace/sken_mdd/inc" -I"C:/Users/hotaka/workspace/sken_mdd/CMSIS/device" -I"C:/Users/hotaka/workspace/sken_mdd/CMSIS/core" -I"C:/Users/hotaka/workspace/sken_mdd/HAL_Driver/Inc" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fno-exceptions -fno-rtti -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


