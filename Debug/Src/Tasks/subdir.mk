################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/Tasks/Tasks.c 

OBJS += \
./Src/Tasks/Tasks.o 

C_DEPS += \
./Src/Tasks/Tasks.d 


# Each subdirectory must supply rules for building sources it contributes
Src/Tasks/%.o Src/Tasks/%.su Src/Tasks/%.cyclo: ../Src/Tasks/%.c Src/Tasks/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32F401CCUx -DSTM32 -DSTM32F4 -c -I../Inc -I"D:/course/embedded/stm32 workspace/RTOS_scheduler/Src/Tasks" -I"D:/course/embedded/stm32 workspace/RTOS_scheduler/Src/OS_MCAL" -I"D:/course/embedded/stm32 workspace/RTOS_scheduler/Src/GPIO" -I"D:/course/embedded/stm32 workspace/RTOS_scheduler/Src/RCC" -I"D:/course/embedded/stm32 workspace/RTOS_scheduler/Src/Kernal" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src-2f-Tasks

clean-Src-2f-Tasks:
	-$(RM) ./Src/Tasks/Tasks.cyclo ./Src/Tasks/Tasks.d ./Src/Tasks/Tasks.o ./Src/Tasks/Tasks.su

.PHONY: clean-Src-2f-Tasks

