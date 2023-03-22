#ifndef INC_STM32F407XX_NVIC_DRIVER_H_
#define INC_STM32F407XX_NVIC_DRIVER_H_
#include"stm32f407xx.h"
/**********************************START:Processor Specific Details **********************************/
/*
 * ARM Cortex Mx Processor NVIC ISERx register Addresses
 */

#define NVIC_ISER0          ((vuint32_t*)0xE000E100)
#define NVIC_ISER1          ((vuint32_t*)0xE000E104)
#define NVIC_ISER2          ((vuint32_t*)0xE000E108)
#define NVIC_ISER3          ((vuint32_t*)0xE000E10c)


/*
 * ARM Cortex Mx Processor NVIC ICERx register Addresses
 */
#define NVIC_ICER0 			((vuint32_t*)0XE000E180)
#define NVIC_ICER1			((vuint32_t*)0XE000E184)
#define NVIC_ICER2  		((vuint32_t*)0XE000E188)
#define NVIC_ICER3			((vuint32_t*)0XE000E18C)


/*
 * ARM Cortex Mx Processor Priority Register Address Calculation
 */
#define NVIC_PR_BASE_ADDR 	((vuint32_t*)0xE000E400)
extern vuint32_t* NVIC_INTR_CTRL_REG_BASE_LST[];
/*
 * ARM Cortex Mx Processor number of priority bits implemented in Priority Register
 */
#define NO_PR_BITS_IMPLEMENTED  4

void NVIC_INTR_CTRL(uint8_t IRQNumber,uint8_t State);
void NVIC_INTR_PR_CTRL(uint8_t IRQNumber,uint8_t IRQPriority);


#endif /* INC_STM32F407XX_NVIC_DRIVER_H_ */
