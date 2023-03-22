################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/src/stm32f407xx.c \
../drivers/src/stm32f407xx_exti_driver.c \
../drivers/src/stm32f407xx_gpio_driver.c \
../drivers/src/stm32f407xx_i2c_driver.c \
../drivers/src/stm32f407xx_nvic_driver.c \
../drivers/src/stm32f407xx_rcc_driver.c \
../drivers/src/stm32f407xx_spi_driver.c \
../drivers/src/stm32f407xx_usart_driver.c 

OBJS += \
./drivers/src/stm32f407xx.o \
./drivers/src/stm32f407xx_exti_driver.o \
./drivers/src/stm32f407xx_gpio_driver.o \
./drivers/src/stm32f407xx_i2c_driver.o \
./drivers/src/stm32f407xx_nvic_driver.o \
./drivers/src/stm32f407xx_rcc_driver.o \
./drivers/src/stm32f407xx_spi_driver.o \
./drivers/src/stm32f407xx_usart_driver.o 

C_DEPS += \
./drivers/src/stm32f407xx.d \
./drivers/src/stm32f407xx_exti_driver.d \
./drivers/src/stm32f407xx_gpio_driver.d \
./drivers/src/stm32f407xx_i2c_driver.d \
./drivers/src/stm32f407xx_nvic_driver.d \
./drivers/src/stm32f407xx_rcc_driver.d \
./drivers/src/stm32f407xx_spi_driver.d \
./drivers/src/stm32f407xx_usart_driver.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/src/%.o drivers/src/%.su drivers/src/%.cyclo: ../drivers/src/%.c drivers/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F4 -DSTM32F407VGTx -DSTM32F407G_DISC1 -DDEBUG -c -I"C:/Users/Prajwal Dikonda/Desktop/STM32 Projects/stm32f4xx_drivers/drivers/inc" -I"C:/Users/Prajwal Dikonda/Desktop/STM32 Projects/stm32f4xx_drivers/bsp" -O0 -ffunction-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"

clean: clean-drivers-2f-src

clean-drivers-2f-src:
	-$(RM) ./drivers/src/stm32f407xx.cyclo ./drivers/src/stm32f407xx.d ./drivers/src/stm32f407xx.o ./drivers/src/stm32f407xx.su ./drivers/src/stm32f407xx_exti_driver.cyclo ./drivers/src/stm32f407xx_exti_driver.d ./drivers/src/stm32f407xx_exti_driver.o ./drivers/src/stm32f407xx_exti_driver.su ./drivers/src/stm32f407xx_gpio_driver.cyclo ./drivers/src/stm32f407xx_gpio_driver.d ./drivers/src/stm32f407xx_gpio_driver.o ./drivers/src/stm32f407xx_gpio_driver.su ./drivers/src/stm32f407xx_i2c_driver.cyclo ./drivers/src/stm32f407xx_i2c_driver.d ./drivers/src/stm32f407xx_i2c_driver.o ./drivers/src/stm32f407xx_i2c_driver.su ./drivers/src/stm32f407xx_nvic_driver.cyclo ./drivers/src/stm32f407xx_nvic_driver.d ./drivers/src/stm32f407xx_nvic_driver.o ./drivers/src/stm32f407xx_nvic_driver.su ./drivers/src/stm32f407xx_rcc_driver.cyclo ./drivers/src/stm32f407xx_rcc_driver.d ./drivers/src/stm32f407xx_rcc_driver.o ./drivers/src/stm32f407xx_rcc_driver.su ./drivers/src/stm32f407xx_spi_driver.cyclo ./drivers/src/stm32f407xx_spi_driver.d ./drivers/src/stm32f407xx_spi_driver.o ./drivers/src/stm32f407xx_spi_driver.su ./drivers/src/stm32f407xx_usart_driver.cyclo ./drivers/src/stm32f407xx_usart_driver.d ./drivers/src/stm32f407xx_usart_driver.o ./drivers/src/stm32f407xx_usart_driver.su

.PHONY: clean-drivers-2f-src

