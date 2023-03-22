#ifndef INC_STM32F407XX_EXTI_DRIVER_H_
#define INC_STM32F407XX_EXTI_DRIVER_H_
#include"stm32f407xx.h"
/*
 * peripheral register definition structure for EXTI
 */
typedef struct {
	vuint32_t IMR; /*!< Give a short description,          	  	    Address offset: 0x00 */
	vuint32_t EMR; /*!< TODO,                						Address offset: 0x04 */
	vuint32_t RTSR; /*!< TODO,  									     Address offset: 0x08 */
	vuint32_t FTSR; /*!< TODO, 										Address offset: 0x0C */
	vuint32_t SWIER; /*!< TODO,  									   Address offset: 0x10 */
	vuint32_t PR; /*!< TODO,                   					   Address offset: 0x14 */

} EXTI_RegDef_t;

#define EXTI_ptr	((EXTI_RegDef_t*)BASE_ADDR_LST[EXTI])

#define EXTI_MASK_CTRL(BITPOS,VALUE)		MODIFY_BIT_N(EXTI_ptr->IMR,BITPOS,VALUE)
#define EXTI_RTSR_CTRL(BITPOS,VALUE)		MODIFY_BIT_N(EXTI_ptr->RTSR,BITPOS,VALUE)
#define EXTI_FTSR_CTRL(BITPOS,VALUE)		MODIFY_BIT_N(EXTI_ptr->FTSR,BITPOS,VALUE)
#define EXTI_PR_CTRL(BITPOS,VALUE)			MODIFY_BIT_N(EXTI_ptr->PR,BITPOS,VALUE)
#define EXTI_INTR_PR_CLR(EXTIx)				SET_BIT_N(EXTI_ptr->PR,EXTIx)//Setting bit value as 1 clears pending status.

#define EXTI_INTR_PR_STATUS(EXTIx)			GET_BIT_N(EXTI_ptr->PR,EXTIx)






#define EXTI0_IRQx	6
#define EXTI1_IRQx	7
#define EXTI2_IRQx	8
#define EXTI3_IRQx	9
#define EXTI4_IRQx	10
#define EXTI5_IRQx	23
#define EXTI6_IRQx	23
#define EXTI7_IRQx	23
#define EXTI8_IRQx	23
#define EXTI9_IRQx	23
#define EXTI10_IRQx	40
#define EXTI11_IRQx	40
#define EXTI12_IRQx	40
#define EXTI13_IRQx	40
#define EXTI14_IRQx	40
#define EXTI15_IRQx	40
#define EXTI16_IRQx	1//PVD through EXTI line detection.EXTI line 16 is connected to the PVD output.
#define EXTI17_IRQx	41//RTC Alarms (A and B) through EXTI line interrupt.EXTI line 17 is connected to the RTC Alarm event.
#define EXTI18_IRQx	42//USB On-The-Go FS Wakeup through EXTI line interrupt.EXTI line 18 is connected to the USB OTG FS Wakeup event.
#define EXTI19_IRQx	62//Ethernet Wakeup through EXTI line interrupt.Ethernet wake up through EXTI line interrupt.EXTI line 19 is connected to the Ethernet Wakeup event.
#define EXTI20_IRQx	76//USB On The Go HS Wakeup through EXTI interrupt.EXTI line 20 is connected to the USB OTG HS (configured in FS) Wakeup event.
#define EXTI21_IRQx	2//Tamper and TimeStamp interrupts through the EXTI line.EXTI line 21 is connected to the RTC Tamper and TimeStamp events.
#define EXTI22_IRQx	3//RTC Wakeup interrupt through the EXTI line.EXTI line 22 is connected to the RTC Wakeup even.


#define EXTI_EDGE_RISING	0
#define EXTI_EDGE_FALLING	1
static void EXTIx_EdgeCONFIG(uint8_t EXTIx,uint8_t EDGEx)
{

}


extern uint8_t EXTIx_IRQx_MAPPING[];



#endif /* INC_STM32F407XX_EXTI_DRIVER_H_ */
