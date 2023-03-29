

#include "stm32f407xx_exti_driver.h"

/*Following functions should not be called from outside.So we declare prototypes here.
 * We can even define them as static.*/
void EXTI_INTR_EDGE_RISING_CONFIG(uint8_t EXTIx);
void EXTI_INTR_EDGE_FALLING_CONFIG(uint8_t EXTIx) ;
void EXTI_INTR_EDGE_BOTH_CONFIG(uint8_t EXTIx) ;
uint8_t EXTIx_IRQx_MAPPING[] = {
EXTI0_IRQx,
EXTI1_IRQx,
EXTI2_IRQx,
EXTI3_IRQx,
EXTI4_IRQx,
EXTI5_IRQx,
EXTI6_IRQx,
EXTI7_IRQx,
EXTI8_IRQx,
EXTI9_IRQx,
EXTI10_IRQx,
EXTI11_IRQx,
EXTI12_IRQx,
EXTI13_IRQx,
EXTI14_IRQx,
EXTI15_IRQx,
EXTI16_IRQx,
EXTI17_IRQx,
EXTI18_IRQx,
EXTI19_IRQx,
EXTI20_IRQx,
EXTI21_IRQx,
EXTI22_IRQx, };

void EXTI_INTR_EDGE_CONFIG(uint8_t EXTIx, uint8_t Edge) {
	void(*fptr[])(uint8_t)={EXTI_INTR_EDGE_FALLING_CONFIG,EXTI_INTR_EDGE_RISING_CONFIG,EXTI_INTR_EDGE_BOTH_CONFIG};
	fptr[Edge](EXTIx);
}
void EXTI_INTR_EDGE_RISING_CONFIG(uint8_t EXTIx) {
	MODIFY_BIT_N(EXTI_ptr->RTSR, EXTIx, 1);
	//Clear the corresponding FTSR bit
	MODIFY_BIT_N(EXTI_ptr->FTSR, EXTIx, 0);

}
void EXTI_INTR_EDGE_FALLING_CONFIG(uint8_t EXTIx) {
	MODIFY_BIT_N(EXTI_ptr->FTSR, EXTIx, 1);
	//Clear the corresponding RTSR bit
	MODIFY_BIT_N(EXTI_ptr->RTSR, EXTIx, 0);
}
void EXTI_INTR_EDGE_BOTH_CONFIG(uint8_t EXTIx) {
	MODIFY_BIT_N(EXTI_ptr->FTSR, EXTIx, 1);
	MODIFY_BIT_N(EXTI_ptr->RTSR, EXTIx, 1);
}
