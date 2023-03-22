#ifndef INC_STM32F407XX_GPIO_DRIVER_H_
#define INC_STM32F407XX_GPIO_DRIVER_H_


#include "stm32f407xx.h"
#include"stm32f407xx_exti_driver.h"
#include"stm32f407xx_rcc_driver.h"
#include "stm32f407xx_syscfg_driver.h"

#define GPIOx_ptr(GPIOx)			((GPIO_RegDef_t*)BASE_ADDR_LST[GPIOx-50])
typedef struct
{
	vuint32_t MODER;                        /*!< GPIO port mode register,                    	Address offset: 0x00      */
	vuint32_t OTYPER;                       /*!< TODO,     										Address offset: 0x04      */
	vuint32_t OSPEEDR;
	vuint32_t PUPDR;
	vuint32_t IDR;
	vuint32_t ODR;
	vuint32_t BSRR;
	vuint32_t LCKR;
	vuint32_t AFR[2];					 /*!< AFR[0] : GPIO alternate function low register, AF[1] : GPIO alternate function high register    		Address offset: 0x20-0x24 */
}GPIO_RegDef_t;

/*
 * This is a Configuration structure for a GPIO pin
 */
typedef struct
{
	uint8_t GPIO_PinNumber;
	uint8_t GPIO_PinMode;			/*!< possible values from @GPIO_PIN_MODES >*/
	uint8_t GPIO_PinSpeed;			/*!< possible values from @GPIO_PIN_SPEED >*/
	uint8_t GPIO_PinPuPdControl;
	uint8_t GPIO_PinOPType;
	uint8_t GPIO_PinAltFunMode;
}GPIO_PinConfig_t;
/*
 * This is a Handle structure for a GPIO pin
 */

typedef struct
{
	uint8_t GPIOx;       		/*!< This holds the base address of the GPIO port to which the pin belongs >*/
	GPIO_PinConfig_t GPIO_PinConfig;   /*!< This holds GPIO pin configuration settings >*/

}GPIO_Handle_t;


/*
 * Clock control Macros for GPIOx peripheral.
 */
#define GPIOx_PCLK_CTRL(GPIOx,BITPOS,STATE)    	MODIFY_BIT_N(RCC_ptr->AHB1ENR,GPIOx-50)


/*
 *  Macros to reset GPIOx peripheral.
 */
#define GPIOx_REG_RESET(GPIOx)    	SET_BIT_N(RCC_ptr->AHB1RSTR,GPIOx-50);RST_BIT_N(RCC_ptr->AHB1RSTR,GPIOx-50)




/*
 * @GPIO_PIN_NUMBERS
 * GPIO pin numbers
 */
#define GPIO_PIN_NO_0  				0
#define GPIO_PIN_NO_1  				1
#define GPIO_PIN_NO_2  				2
#define GPIO_PIN_NO_3  				3
#define GPIO_PIN_NO_4  				4
#define GPIO_PIN_NO_5  				5
#define GPIO_PIN_NO_6  				6
#define GPIO_PIN_NO_7  				7
#define GPIO_PIN_NO_8  				8
#define GPIO_PIN_NO_9  				9
#define GPIO_PIN_NO_10  			10
#define GPIO_PIN_NO_11 				11
#define GPIO_PIN_NO_12  			12
#define GPIO_PIN_NO_13 				13
#define GPIO_PIN_NO_14 				14
#define GPIO_PIN_NO_15 				15

/*
 * @GPIO_PIN_MODES
 * GPIO pin possible modes
 */
#define GPIO_MODE_IN 		0
#define GPIO_MODE_OUT 		1
#define GPIO_MODE_ALTFN 	2
#define GPIO_MODE_ANALOG 	3
#define GPIO_MODE_IT_FT     4
#define GPIO_MODE_IT_RT     5
#define GPIO_MODE_IT_RFT    6


/*
 * GPIO pin possible output types
 */
#define GPIO_OP_TYPE_PP   0
#define GPIO_OP_TYPE_OD   1


/*
 * @GPIO_PIN_SPEED
 * GPIO pin possible output speeds
 */
#define GPIO_SPEED_LOW			0
#define GPIO_SPEED_MEDIUM		1
#define GPIO_SPEED_FAST			2
#define GPOI_SPEED_HIGH			3


/*
 * GPIO pin pull up AND pull down configuration macros
 */
#define GPIO_NO_PUPD   		0
#define GPIO_PIN_PU			1
#define GPIO_PIN_PD			2

/******************************************************************************************
 *								APIs supported by this driver
 *		 For more information about the APIs check the function definitions
 ******************************************************************************************/

/*
 * Peripheral Clock setup
 */
void GPIO_PeriClockControl(uint8_t GPIOx, uint8_t State);

/*
 * Init and De-init
 */
void GPIO_Init(GPIO_Handle_t GPIOHandle);
void GPIO_DeInit(uint8_t GPIOx);


/*
 * Data read and write
 */
uint8_t GPIO_ReadFromInputPin(uint8_t GPIOx, uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(uint8_t GPIOx);
void GPIO_WriteToOutputPin(uint8_t GPIOx, uint8_t PinNumber, uint8_t Value);
void GPIO_WriteToOutputPort(uint8_t GPIOx, uint16_t Value);
void GPIO_ToggleOutputPin(uint8_t GPIOx, uint8_t PinNumber);


/*
 * IRQ Configuration and ISR handling
 */
void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t State);
void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void GPIO_IRQHandling(uint8_t PinNumber);






#endif /* INC_STM32F407XX_GPIO_DRIVER_H_ */
