/*
 * stm32f407xx_rcc_driver.h
 *
 *  Created on: Mar 29, 2019
 *      Author: admin
 */

#ifndef INC_STM32F407XX_RCC_DRIVER_H_
#define INC_STM32F407XX_RCC_DRIVER_H_

#include "stm32f407xx.h"

/*
 * peripheral register definition structure for RCC
 */
typedef struct {
	vuint32_t CR; /*!< TODO,     										Address offset: 0x00 */
	vuint32_t PLLCFGR; /*!< TODO,     										Address offset: 0x04 */
	vuint32_t CFGR; /*!< TODO,     										Address offset: 0x08 */
	vuint32_t CIR; /*!< TODO,     										Address offset: 0x0C */
	vuint32_t AHB1RSTR; /*!< TODO,     										Address offset: 0x10 */
	vuint32_t AHB2RSTR; /*!< TODO,     										Address offset: 0x14 */
	vuint32_t AHB3RSTR; /*!< TODO,     										Address offset: 0x18 */
	uint32_t RESERVED0; /*!< Reserved, 0x1C                                                       */
	vuint32_t APB1RSTR; /*!< TODO,     										Address offset: 0x20 */
	vuint32_t APB2RSTR; /*!< TODO,     										Address offset: 0x24 */
	uint32_t RESERVED1[2]; /*!< Reserved, 0x28-0x2C                                                  */
	vuint32_t AHB1ENR; /*!< TODO,     										Address offset: 0x30 */
	vuint32_t AHB2ENR; /*!< TODO,     										Address offset: 0x34 */
	vuint32_t AHB3ENR; /*!< TODO,     										Address offset: 0x38 */
	uint32_t RESERVED2; /*!< Reserved, 0x3C                                                       */
	vuint32_t APB1ENR; /*!< TODO,     										Address offset: 0x40 */
	vuint32_t APB2ENR; /*!< TODO,     										Address offset: 0x44 */
	uint32_t RESERVED3[2]; /*!< Reserved, 0x48-0x4C                                                  */
	vuint32_t AHB1LPENR; /*!< TODO,     										Address offset: 0x50 */
	vuint32_t AHB2LPENR; /*!< TODO,     										Address offset: 0x54 */
	vuint32_t AHB3LPENR; /*!< TODO,     										Address offset: 0x58 */
	uint32_t RESERVED4; /*!< Reserved, 0x5C                                                       */
	vuint32_t APB1LPENR; /*!< TODO,     										Address offset: 0x60 */
	vuint32_t APB2LPENR; /*!< RTODO,     										Address offset: 0x64 */
	uint32_t RESERVED5[2]; /*!< Reserved, 0x68-0x6C                                                  */
	vuint32_t BDCR; /*!< TODO,     										Address offset: 0x70 */
	vuint32_t CSR; /*!< TODO,     										Address offset: 0x74 */
	uint32_t RESERVED6[2]; /*!< Reserved, 0x78-0x7C                                                  */
	vuint32_t SSCGR; /*!< TODO,     										Address offset: 0x80 */
	vuint32_t PLLI2SCFGR; /*!< TODO,     										Address offset: 0x84 */
	vuint32_t PLLSAICFGR; /*!< TODO,     										Address offset: 0x88 */
	vuint32_t DCKCFGR; /*!< TODO,     										Address offset: 0x8C */
	vuint32_t CKGATENR; /*!< TODO,     										Address offset: 0x90 */
	vuint32_t DCKCFGR2; /*!< TODO,     										Address offset: 0x94 */

} RCC_RegDef_t;

#define RCC_ptr	((RCC_RegDef_t*)BASE_ADDR_LST[RCC])
#define RCC_AHB1_PER_CLK_CTRL(BITPOS,STATE)	 MODIFY_BIT_N(RCC_ptr->AHB1ENR,BITPOS,STATE)
#define RCC_APB1_PER_CLK_CTRL(BITPOS,STATE)	 MODIFY_BIT_N(RCC_ptr->APB1ENR,BITPOS,STATE)
#define RCC_APB2_PER_CLK_CTRL(BITPOS,STATE)	 MODIFY_BIT_N(RCC_ptr->APB2ENR,BITPOS,STATE)

#define RCC_AHB1_PER_RST(BITPOS)	SET_BIT_N(RCC_ptr->AHB1RSTR,BITPOS);RST_BIT_N(RCC_ptr->AHB1RSTR,BITPOS)
#define RCC_APB1_PER_RST(BITPOS)	 SET_BIT_N(RCC_ptr->APB1RSTR,BITPOS);RST_BIT_N(RCC_ptr->APB1RSTR,BITPOS)
#define RCC_APB2_PER_RST(BITPOS)	 SET_BIT_N(RCC_ptr->APB2RSTR,BITPOS);RST_BIT_N(RCC_ptr->APB2RSTR,BITPOS)
//This returns the APB1 clock value
uint32_t RCC_GetPCLK1Value(void);

//This returns the APB2 clock value
uint32_t RCC_GetPCLK2Value(void);

uint32_t RCC_GetPLLOutputClock(void);
#endif /* INC_STM32F407XX_RCC_DRIVER_H_ */
