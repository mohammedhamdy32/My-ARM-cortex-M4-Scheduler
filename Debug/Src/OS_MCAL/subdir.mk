################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/OS_MCAL/OS_MCAL_program.c 

OBJS += \
./Src/OS_MCAL/OS_MCAL_program.o 

C_DEPS += \
./Src/OS_MCAL/OS_MCAL_program.d 


# Each subdirectory must supply rules for building sources it contributes
Src/OS_MCAL/%.o Src/OS_MCAL/%.su Src/OS_MCAL/%.cyclo: ../Src/OS_MCAL/%.c Src/OS_MCAL/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32F401CCUx -DSTM32 -DSTM32F4 -c -I../Inc -I"D:/course/embedded/stm32 workspace/RTOS_scheduler/Src/Tasks" -I"D:/course/embedded/stm32 workspace/RTOS_scheduler/Src/OS_MCAL" -I"D:/course/embedded/stm32 workspace/RTOS_scheduler/Src/GPIO" -I"D:/course/embedded/stm32 workspace/RTOS_scheduler/Src/RCC" -I"D:/course/embedded/stm32 workspace/RTOS_scheduler/Src/Kernal" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src-2f-OS_MCAL

clean-Src-2f-OS_MCAL:
	-$(RM) ./Src/OS_MCAL/OS_MCAL_program.cyclo ./Src/OS_MCAL/OS_MCAL_program.d ./Src/OS_MCAL/OS_MCAL_program.o ./Src/OS_MCAL/OS_MCAL_program.su

.PHONY: clean-Src-2f-OS_MCAL

