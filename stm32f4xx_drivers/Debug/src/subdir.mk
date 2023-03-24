################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/005button_interrupt.c 

OBJS += \
./src/005button_interrupt.o 

C_DEPS += \
./src/005button_interrupt.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o src/%.su src/%.cyclo: ../src/%.c src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F4 -DSTM32F407VGTx -DSTM32F407G_DISC1 -DDEBUG -c -I"C:/Users/Prajwal Dikonda/Desktop/STM32 Projects/stm32f4xx_drivers/drivers/inc" -I"C:/Users/Prajwal Dikonda/Desktop/STM32 Projects/stm32f4xx_drivers/bsp" -O0 -ffunction-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"

clean: clean-src

clean-src:
	-$(RM) ./src/005button_interrupt.cyclo ./src/005button_interrupt.d ./src/005button_interrupt.o ./src/005button_interrupt.su

.PHONY: clean-src

