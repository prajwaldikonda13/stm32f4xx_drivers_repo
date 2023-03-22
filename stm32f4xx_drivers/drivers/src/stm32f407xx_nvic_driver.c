#include"stm32f407xx_nvic_driver.h"

void NVIC_INTR_CTRL(uint8_t IRQNumber,uint8_t State)
{
	vuint32_t* NVIC_INTR_CTRL_REG_BASE_LST[]={NVIC_ICER0,NVIC_ISER0};
	SET_BIT_N(*(NVIC_INTR_CTRL_REG_BASE_LST[State]+DIV2PK(IRQNumber,5)),MOD2PK(IRQNumber,5));
}
void NVIC_INTR_PR_CTRL(uint8_t IRQNumber,uint8_t IRQPriority)
{
	MODIFY_KBITS(*( NVIC_PR_BASE_ADDR + DIV2PK(IRQNumber, 2)),4,MOD2PK(IRQNumber, 2)+4,IRQPriority);
}
