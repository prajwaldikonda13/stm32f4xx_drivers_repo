#ifndef INC_STM32F407XX_SYSCFG_DRIVER_H_
#define INC_STM32F407XX_SYSCFG_DRIVER_H_

#include"stm32f407xx_rcc_driver.h"

/*
 * peripheral register definition structure for SYSCFG
 */
typedef struct
{
	vuint32_t MEMRMP;       /*!< Give a short description,                    Address offset: 0x00      */
	vuint32_t PMC;          /*!< TODO,     									  Address offset: 0x04      */
	vuint32_t EXTICR[4];    /*!< TODO , 									  Address offset: 0x08-0x14 */
	uint32_t      RESERVED1[2];  /*!< TODO          							  Reserved, 0x18-0x1C    	*/
	vuint32_t CMPCR;        /*!< TODO         								  Address offset: 0x20      */
	uint32_t      RESERVED2[2];  /*!<                                             Reserved, 0x24-0x28 	    */
	vuint32_t CFGR;         /*!< TODO                                         Address offset: 0x2C   	*/
} SYSCFG_RegDef_t;
#define SYSCFG_ptr				((SYSCFG_RegDef_t*)BASE_ADDR_LST[SYSCFG])


#define SYSCFG_EXTI_CNFG(GPIOPINx)	MODIFY_KBITS(SYSCFG_ptr->EXTICR[DIV2PK(GPIOPINx, 2)],4, MUL2PK(MOD2PK(GPIOPINx,2),2),GPIOPINx);
/*
 * Clock Enable Macros for SYSCFG peripheral
 */
#define SYSCFG_PCLK_EN() 			RCC_APB1_PER_CLK_CTRL(14,ENABLE)
//#define SYSCFG_PCLK_EN() (RCC_ptr->APB2ENR |= (1 << 14))


#endif /* INC_STM32F407XX_SYSCFG_DRIVER_H_ */
