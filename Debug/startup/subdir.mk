################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../startup/startup_stm32f446xx.s 

OBJS += \
./startup/startup_stm32f446xx.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo $(PWD)
	arm-none-eabi-as -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -I"C:/Users/hotaka/workspace/sken_mdd/HAL_Driver/Inc/Legacy" -I"C:/Users/hotaka/workspace/sken_mdd/Utilities/STM32F4xx-Nucleo" -I"C:/Users/hotaka/workspace/sken_mdd/inc" -I"C:/Users/hotaka/workspace/sken_mdd/CMSIS/device" -I"C:/Users/hotaka/workspace/sken_mdd/CMSIS/core" -I"C:/Users/hotaka/workspace/sken_mdd/HAL_Driver/Inc" -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


